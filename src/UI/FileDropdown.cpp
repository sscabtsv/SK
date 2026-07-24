#include "FileDropdown.hpp"
#include "Theme.hpp"
#include "../Macro/MacroFile.hpp"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace geode::prelude;

namespace showcasekit {

FileDropdown* FileDropdown::create(CCSize const& size, std::function<void(std::string)> onSelect) {
    auto ret = new FileDropdown();
    if (ret && ret->init(size, std::move(onSelect))) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool FileDropdown::init(CCSize const& size, std::function<void(std::string)> onSelect) {
    if (!CCMenu::init()) return false;

    m_onSelect = std::move(onSelect);
    m_closedSize = size;
    this->setContentSize(size);
    this->setAnchorPoint({0.5f, 0.5f});

    m_panelBg = CCScale9Sprite::create("square02_small.png");
    m_panelBg->setContentSize(size);
    m_panelBg->setColor(theme::kPanelLight);
    m_panelBg->setOpacity(255);
    m_panelBg->setPosition({size.width / 2.f, size.height / 2.f});
    this->addChild(m_panelBg, -1);

    m_headerLabel = CCLabelBMFont::create("No macro selected", "bigFont.fnt");
    m_headerLabel->setScale(0.42f);
    m_headerLabel->setAnchorPoint({0.f, 0.5f});
    m_headerLabel->setPosition({10.f, size.height / 2.f});
    this->addChild(m_headerLabel);

    auto arrow = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    if (arrow) {
        arrow->setScale(0.5f);
        arrow->setRotation(90.f);
    }
    auto headerSprite = CCMenuItemSpriteExtra::create(
        arrow ? static_cast<CCNode*>(arrow) : static_cast<CCNode*>(CCLabelBMFont::create("v", "bigFont.fnt")),
        this,
        menu_selector(FileDropdown::onToggle)
    );
    headerSprite->setPosition({size.width - 16.f, size.height / 2.f});
    this->addChild(headerSprite);
    m_headerButton = headerSprite;

    m_scrollLayer = ScrollLayer::create({size.width, 100.f});
    m_scrollLayer->setPosition({0.f, -104.f});
    m_scrollLayer->setVisible(false);
    this->addChild(m_scrollLayer);

    this->refresh();
    return true;
}

void FileDropdown::refresh() {
    rebuildList();
}

void FileDropdown::rebuildList() {
    if (!m_scrollLayer || !m_scrollLayer->m_contentLayer) return;
    m_scrollLayer->m_contentLayer->removeAllChildren();

    auto names = MacroFile::listSavedMacros();
    float rowHeight = 24.f;
    float y = 0.f;

    for (size_t i = 0; i < names.size(); ++i) {
        auto row = CCMenu::create();
        row->setContentSize({m_closedSize.width, rowHeight});
        row->setPosition({0.f, static_cast<float>(names.size() - 1 - i) * rowHeight});
        row->setAnchorPoint({0.f, 0.f});

        auto label = CCLabelBMFont::create(names[i].c_str(), "chatFont.fnt");
        label->setScale(0.5f);
        label->setAnchorPoint({0.f, 0.5f});

        auto item = CCMenuItemSpriteExtra::create(
            label,
            this,
            menu_selector(FileDropdown::onPick)
        );
        item->setAnchorPoint({0.f, 0.5f});
        item->setPosition({10.f, rowHeight / 2.f});
        item->setUserObject(CCString::create(names[i]));
        row->addChild(item);

        m_scrollLayer->m_contentLayer->addChild(row);
        y += rowHeight;
    }

    m_scrollLayer->m_contentLayer->setContentSize({m_closedSize.width, std::max(y, 100.f)});
}

void FileDropdown::setOpen(bool open) {
    m_open = open;
    m_scrollLayer->setVisible(open);
}

void FileDropdown::onToggle(CCObject*) {
    rebuildList();
    setOpen(!m_open);
}

void FileDropdown::onPick(CCObject* sender) {
    auto item = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto str = static_cast<CCString*>(item->getUserObject());
    if (!str) return;

    setSelected(str->getCString());
    setOpen(false);

    if (m_onSelect) m_onSelect(m_selected);
}

void FileDropdown::setSelected(std::string const& name) {
    m_selected = name;
    m_headerLabel->setString(name.empty() ? "No macro selected" : name.c_str());
}

}
