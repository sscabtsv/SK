#include <Geode/modify/PauseLayer.hpp>
#include "../UI/MainPopup.hpp"
#include "../UI/Theme.hpp"

using namespace geode::prelude;
using namespace showcasekit;

class $modify(SKPauseLayerHook, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto menu = this->getChildByID("left-button-menu");
        if (!menu) return;

        auto sprite = CircleButtonSprite::createWithSpriteFrameName(
            "SK_icon.png"_spr,
            1.f,
            CircleBaseColor::Green,
            CircleBaseSize::Medium
        );

        if (!sprite) {
            auto fallbackIcon = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
            sprite = CircleButtonSprite::create(fallbackIcon);
        }

        auto btn = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(SKPauseLayerHook::onOpenShowcaseKit)
        );
        btn->setID("showcasekit-button"_spr);

        menu->addChild(btn);
        menu->updateLayout();
    }

    void onOpenShowcaseKit(CCObject*) {
        MainPopup::create()->show();
    }
};
