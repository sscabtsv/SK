#pragma once

#include "MacroData.hpp"
#include <filesystem>
#include <Geode/Result.hpp>

namespace showcasekit {

class MacroFile {
public:
    static geode::Result<> save(MacroData const& data, std::filesystem::path const& path);
    static geode::Result<MacroData> load(std::filesystem::path const& path);
    static std::filesystem::path macroDirectory();
    static std::vector<std::string> listSavedMacros();
};

}
