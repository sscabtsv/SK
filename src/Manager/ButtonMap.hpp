#pragma once

#include <Geode/Geode.hpp>
#include "../Macro/MacroTypes.hpp"

namespace showcasekit {

inline PlayerButton toPlayerButton(MacroButton button) {
    switch (button) {
        case MacroButton::Jump: return PlayerButton::Jump;
        case MacroButton::Left: return PlayerButton::Left;
        case MacroButton::Right: return PlayerButton::Right;
    }
    return PlayerButton::Jump;
}

inline MacroButton fromPlayerButton(PlayerButton button) {
    switch (button) {
        case PlayerButton::Left: return MacroButton::Left;
        case PlayerButton::Right: return MacroButton::Right;
        default: return MacroButton::Jump;
    }
}

}
