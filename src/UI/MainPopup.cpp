#include "MainPopup.hpp"
#include "Theme.hpp"

using namespace cocos2d;
using namespace geode::prelude;

namespace showcasekit {

MainPopup* MainPopup::create() {
    auto ret = new MainPopup();
    if (ret && ret->initAnchored(theme::kPopupWidth, theme::kPopupHeight)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MainPopup::setup() {
    this->setTitle("ShowcaseKit");

    auto size = m_mainLayer->getContentSize();

    auto bg = CCLayerColor::create({
        theme::kPanel.r, theme::kPanel.g, theme::kPanel.b, 255
    });
    bg->setContentSize({size.width - 20.f, size.height - 55.f});
    bg->setPosition({10.f, 10.f});
    m_mainLayer->addChild(bg, -1);

    m_macroTab = MacroTab::create({size.width - 20.f, size.height - 55.f});
    m_macroTab->setPosition({10.f, 10.f});
    m_mainLayer->addChild(m_macroTab);

    return true;
}

}
