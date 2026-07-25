#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/ui/Popup.hpp>
#include <functional>

namespace showcasekit {

class SaveMacroPopup : public geode::Popup {
protected:
    geode::TextInput* m_input = nullptr;
    std::function<void(std::string)> m_callback;

    bool init(std::function<void(std::string)> callback);
    void onConfirm(cocos2d::CCObject*);

public:
    static SaveMacroPopup* create(std::function<void(std::string)> callback);
};

}
