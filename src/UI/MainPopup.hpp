#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include "MacroTab.hpp"

namespace showcasekit {

class MainPopup : public geode::Popup {
protected:
    MacroTab* m_macroTab = nullptr;

    bool init();

public:
    static MainPopup* create();
};

}
