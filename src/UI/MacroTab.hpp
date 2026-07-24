#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/TextInput.hpp>
#include "FileDropdown.hpp"

namespace showcasekit {

class MacroTab : public cocos2d::CCLayer {
protected:
    cocos2d::CCMenu* m_menu = nullptr;
    cocos2d::CCMenuItemToggler* m_recordToggle = nullptr;
    cocos2d::CCMenuItemToggler* m_playToggle = nullptr;
    cocos2d::CCLabelBMFont* m_statusLabel = nullptr;
    FileDropdown* m_dropdown = nullptr;

    cocos2d::CCMenuItemToggler* m_showcaseToggle = nullptr;
    geode::TextInput* m_attemptsInput = nullptr;
    geode::TextInput* m_minProgressInput = nullptr;
    geode::TextInput* m_maxProgressInput = nullptr;
    geode::TextInput* m_seedInput = nullptr;

    bool init(cocos2d::CCSize const& size);
    void updateStatus(float dt);

    void onRecordToggle(cocos2d::CCObject*);
    void onPlayToggle(cocos2d::CCObject*);
    void onSave(cocos2d::CCObject*);
    void onShowcaseToggle(cocos2d::CCObject*);
    void onStartShowcase(cocos2d::CCObject*);

public:
    static MacroTab* create(cocos2d::CCSize const& size);
};

}
