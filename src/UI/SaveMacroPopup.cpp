#include "SaveMacroPopup.hpp"
#include "Theme.hpp"

using namespace cocos2d;
using namespace geode::prelude;

namespace showcasekit {

SaveMacroPopup* SaveMacroPopup::create(std::function<void(std::string)> callback) {
    auto ret = new SaveMacroPopup();
    if (ret && ret->initAnchored(240.f, 140.f, callback)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SaveMacroPopup::setup(std::function<void(std::string)> callback) {
    m_callback = std::move(callback);
    this->setTitle("Save Macro");

    auto center = m_mainLayer->getContentSize() / 2.f;

    m_input = TextInput::create(200.f, "macro name");
    m_input->setPosition({center.width, center.height + 5.f});
    m_input->setCommonFilter(CommonFilter::Any);
    m_mainLayer->addChild(m_input);

    auto confirmSprite = ButtonSprite::create("Save");
    auto confirmBtn = CCMenuItemSpriteExtra::create(
        confirmSprite,
        this,
        menu_selector(SaveMacroPopup::onConfirm)
    );
    confirmBtn->setPosition({center.width, center.height - 35.f});

    auto menu = CCMenu::create();
    menu->addChild(confirmBtn);
    menu->setPosition({0.f, 0.f});
    m_mainLayer->addChild(menu);

    return true;
}

void SaveMacroPopup::onConfirm(CCObject*) {
    auto name = m_input->getString();
    if (!name.empty() && m_callback) {
        m_callback(name);
    }
    this->onClose(nullptr);
}

}
