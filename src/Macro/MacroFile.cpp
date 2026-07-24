#include "MacroFile.hpp"
#include <Geode/Geode.hpp>
#include <fstream>

using namespace geode::prelude;

namespace showcasekit {

constexpr uint32_t kMagic = 0x31524447u;
constexpr uint8_t kFormatVersion = 1;

static void writeString(std::ofstream& out, std::string const& str) {
    uint16_t len = static_cast<uint16_t>(std::min<size_t>(str.size(), 65535));
    out.write(reinterpret_cast<char const*>(&len), sizeof(len));
    out.write(str.data(), len);
}

static bool readString(std::ifstream& in, std::string& out) {
    uint16_t len = 0;
    in.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (!in.good()) return false;
    out.resize(len);
    if (len > 0) {
        in.read(out.data(), len);
    }
    return in.good() || len == 0;
}

static uint8_t packEvent(MacroEvent const& ev) {
    uint8_t packed = static_cast<uint8_t>(ev.button) & 0x03;
    if (ev.player2) packed |= 0x04;
    if (ev.push) packed |= 0x08;
    return packed;
}

static MacroEvent unpackEvent(uint32_t frame, uint8_t packed) {
    MacroEvent ev;
    ev.frame = frame;
    ev.button = static_cast<MacroButton>(packed & 0x03);
    ev.player2 = (packed & 0x04) != 0;
    ev.push = (packed & 0x08) != 0;
    return ev;
}

std::filesystem::path MacroFile::macroDirectory() {
    auto dir = Mod::get()->getSaveDir() / "macros";
    if (!std::filesystem::exists(dir)) {
        std::error_code ec;
        std::filesystem::create_directories(dir, ec);
    }
    return dir;
}

std::vector<std::string> MacroFile::listSavedMacros() {
    std::vector<std::string> names;
    std::error_code ec;
    auto dir = macroDirectory();
    if (!std::filesystem::exists(dir, ec)) return names;

    for (auto const& entry : std::filesystem::directory_iterator(dir, ec)) {
        if (!entry.is_regular_file()) continue;
        if (entry.path().extension() != ".gdr") continue;
        names.push_back(entry.path().stem().string());
    }
    std::sort(names.begin(), names.end());
    return names;
}

Result<> MacroFile::save(MacroData const& data, std::filesystem::path const& path) {
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out.is_open()) {
        return Err("Could not open {} for writing", path.string());
    }

    out.write(reinterpret_cast<char const*>(&kMagic), sizeof(kMagic));
    out.write(reinterpret_cast<char const*>(&kFormatVersion), sizeof(kFormatVersion));

    auto const& meta = data.metadata();
    writeString(out, meta.name);
    writeString(out, meta.levelName);

    uint32_t totalFrames = data.lastFrame();
    out.write(reinterpret_cast<char const*>(&totalFrames), sizeof(totalFrames));
    out.write(reinterpret_cast<char const*>(&meta.fps), sizeof(meta.fps));

    uint32_t count = static_cast<uint32_t>(data.eventCount());
    out.write(reinterpret_cast<char const*>(&count), sizeof(count));

    for (auto const& ev : data.getEvents()) {
        out.write(reinterpret_cast<char const*>(&ev.frame), sizeof(ev.frame));
        uint8_t packed = packEvent(ev);
        out.write(reinterpret_cast<char const*>(&packed), sizeof(packed));
    }

    if (!out.good()) {
        return Err("Write failure while saving {}", path.string());
    }

    return Ok();
}

Result<MacroData> MacroFile::load(std::filesystem::path const& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in.is_open()) {
        return Err("Could not open {} for reading", path.string());
    }

    uint32_t magic = 0;
    in.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    if (!in.good() || magic != kMagic) {
        return Err("{} is not a valid ShowcaseKit macro file", path.string());
    }

    uint8_t version = 0;
    in.read(reinterpret_cast<char*>(&version), sizeof(version));
    if (version != kFormatVersion) {
        return Err("Unsupported macro file version {}", static_cast<int>(version));
    }

    MacroData data;
    auto& meta = data.metadata();

    if (!readString(in, meta.name)) {
        return Err("Corrupt macro file: bad name field");
    }
    if (!readString(in, meta.levelName)) {
        return Err("Corrupt macro file: bad level name field");
    }

    uint32_t totalFrames = 0;
    in.read(reinterpret_cast<char*>(&totalFrames), sizeof(totalFrames));
    meta.totalFrames = totalFrames;

    in.read(reinterpret_cast<char*>(&meta.fps), sizeof(meta.fps));

    uint32_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (!in.good()) {
        return Err("Corrupt macro file: bad event count");
    }

    for (uint32_t i = 0; i < count; ++i) {
        uint32_t frame = 0;
        uint8_t packed = 0;
        in.read(reinterpret_cast<char*>(&frame), sizeof(frame));
        in.read(reinterpret_cast<char*>(&packed), sizeof(packed));
        if (!in.good()) {
            return Err("Corrupt macro file: truncated event {}", i);
        }
        data.addEvent(unpackEvent(frame, packed));
    }

    data.sortIfNeeded();
    return Ok(data);
}

}
