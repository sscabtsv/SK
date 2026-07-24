#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/ui/Popup.hpp>
#include <functional>

namespace showcasekit {

class SaveMacroPopup : public geode::Popup<std::function<void(std::string)>> {
protected:
    geode::TextInput* m_input = nullptr;
    std::function<void(std::string)> m_callback;

    bool setup(std::function<void(std::string)> callback) override;
    void onConfirm(cocos2d::CCObject*);

public:
    static SaveMacroPopup* create(std::function<void(std::string)> callback);
};

}
