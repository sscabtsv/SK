#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace showcasekit {

enum class MacroButton : uint8_t {
    Jump = 0,
    Left = 1,
    Right = 2
};

struct MacroEvent {
    uint32_t frame = 0;
    MacroButton button = MacroButton::Jump;
    bool player2 = false;
    bool push = false;

    bool operator<(MacroEvent const& other) const {
        return frame < other.frame;
    }
};

struct MacroMetadata {
    std::string name;
    std::string levelName;
    uint32_t totalFrames = 0;
    uint32_t fps = 240;
};

struct HeldButtonState {
    bool jump1 = false;
    bool left1 = false;
    bool right1 = false;
    bool jump2 = false;
    bool left2 = false;
    bool right2 = false;
};

}
