#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <functional>

namespace showcasekit {

class FileDropdown : public cocos2d::CCMenu {
protected:
    cocos2d::CCLabelBMFont* m_headerLabel = nullptr;
    geode::ScrollLayer* m_scrollLayer = nullptr;
    cocos2d::extension::CCScale9Sprite* m_panelBg = nullptr;
    CCMenuItemSpriteExtra* m_headerButton = nullptr;
    std::function<void(std::string)> m_onSelect;
    std::string m_selected;
    bool m_open = false;
    cocos2d::CCSize m_closedSize;

    bool init(cocos2d::CCSize const& size, std::function<void(std::string)> onSelect);
    void rebuildList();
    void setOpen(bool open);
    void onToggle(cocos2d::CCObject*);
    void onPick(cocos2d::CCObject* sender);

public:
    static FileDropdown* create(cocos2d::CCSize const& size, std::function<void(std::string)> onSelect);
    void refresh();
    void setSelected(std::string const& name);
    std::string const& getSelected() const { return m_selected; }
};

}
