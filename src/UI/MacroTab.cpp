#include "MacroTab.hpp"
#include "SaveMacroPopup.hpp"
#include "Theme.hpp"
#include "../Manager/BotManager.hpp"

using namespace cocos2d;
using namespace geode::prelude;

namespace showcasekit {

MacroTab* MacroTab::create(CCSize const& size) {
    auto ret = new MacroTab();
    if (ret && ret->init(size)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MacroTab::init(CCSize const& size) {
    if (!CCLayer::init()) return false;

    m_menu = CCMenu::create();
    m_menu->setPosition({0.f, 0.f});
    this->addChild(m_menu);

    float top = size.height - 20.f;

    auto recordOffSpr = CCSprite::createWithSpriteFrameName("GJ_stopEditorBtn_001.png");
    auto recordOnSpr = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
    m_recordToggle = CCMenuItemToggler::create(
        recordOffSpr ? recordOffSpr : CCSprite::create(),
        recordOnSpr ? recordOnSpr : CCSprite::create(),
        this,
        menu_selector(MacroTab::onRecordToggle)
    );
    m_recordToggle->setPosition({30.f, top - 10.f});
    m_menu->addChild(m_recordToggle);

    auto playOffSpr = CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png");
    auto playOnSpr = CCSprite::createWithSpriteFrameName("GJ_pauseBtn_001.png");
    m_playToggle = CCMenuItemToggler::create(
        playOffSpr ? playOffSpr : CCSprite::create(),
        playOnSpr ? playOnSpr : CCSprite::create(),
        this,
        menu_selector(MacroTab::onPlayToggle)
    );
    m_playToggle->setPosition({80.f, top - 10.f});
    m_menu->addChild(m_playToggle);

    auto saveSprite = ButtonSprite::create("Save", "bigFont.fnt", "GJ_button_04.png", 0.8f);
    auto saveBtn = CCMenuItemSpriteExtra::create(
        saveSprite,
        this,
        menu_selector(MacroTab::onSave)
    );
    saveBtn->setPosition({size.width - 40.f, top - 10.f});
    m_menu->addChild(saveBtn);

    m_statusLabel = CCLabelBMFont::create("Idle", "chatFont.fnt");
    m_statusLabel->setScale(0.5f);
    m_statusLabel->setAnchorPoint({0.f, 0.5f});
    m_statusLabel->setPosition({130.f, top - 10.f});
    this->addChild(m_statusLabel);

    m_dropdown = FileDropdown::create({size.width - 20.f, 28.f}, [](std::string name) {
        BotManager::get()->loadMacro(name);
    });
    m_dropdown->setPosition({size.width / 2.f, top - 40.f});
    this->addChild(m_dropdown);

    float showcaseY = top - 90.f;

    auto showcaseLabel = CCLabelBMFont::create("Showcase Attempts", "goldFont.fnt");
    showcaseLabel->setScale(0.45f);
    showcaseLabel->setAnchorPoint({0.f, 0.5f});
    showcaseLabel->setPosition({10.f, showcaseY});
    this->addChild(showcaseLabel);

    auto toggleOffSpr = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    auto toggleOnSpr = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    m_showcaseToggle = CCMenuItemToggler::create(
        toggleOffSpr ? toggleOffSpr : CCSprite::create(),
        toggleOnSpr ? toggleOnSpr : CCSprite::create(),
        this,
        menu_selector(MacroTab::onShowcaseToggle)
    );
    m_showcaseToggle->setPosition({size.width - 30.f, showcaseY});
    m_menu->addChild(m_showcaseToggle);

    float rowY = showcaseY - 30.f;

    auto attemptsLabel = CCLabelBMFont::create("Attempts", "chatFont.fnt");
    attemptsLabel->setScale(0.4f);
    attemptsLabel->setAnchorPoint({0.f, 0.5f});
    attemptsLabel->setPosition({10.f, rowY});
    this->addChild(attemptsLabel);

    m_attemptsInput = TextInput::create(50.f, "3");
    m_attemptsInput->setCommonFilter(CommonFilter::Int);
    m_attemptsInput->setString("3");
    m_attemptsInput->setPosition({100.f, rowY});
    this->addChild(m_attemptsInput);

    auto rangeLabel = CCLabelBMFont::create("Range %", "chatFont.fnt");
    rangeLabel->setScale(0.4f);
    rangeLabel->setAnchorPoint({0.f, 0.5f});
    rangeLabel->setPosition({150.f, rowY});
    this->addChild(rangeLabel);

    m_minProgressInput = TextInput::create(40.f, "5");
    m_minProgressInput->setCommonFilter(CommonFilter::Float);
    m_minProgressInput->setString("5");
    m_minProgressInput->setPosition({225.f, rowY});
    this->addChild(m_minProgressInput);

    m_maxProgressInput = TextInput::create(40.f, "90");
    m_maxProgressInput->setCommonFilter(CommonFilter::Float);
    m_maxProgressInput->setString("90");
    m_maxProgressInput->setPosition({270.f, rowY});
    this->addChild(m_maxProgressInput);

    float rowY2 = rowY - 28.f;

    auto seedLabel = CCLabelBMFont::create("Seed (opt.)", "chatFont.fnt");
    seedLabel->setScale(0.4f);
    seedLabel->setAnchorPoint({0.f, 0.5f});
    seedLabel->setPosition({10.f, rowY2});
    this->addChild(seedLabel);

    m_seedInput = TextInput::create(60.f, "random");
    m_seedInput->setCommonFilter(CommonFilter::Int);
    m_seedInput->setPosition({110.f, rowY2});
    this->addChild(m_seedInput);

    auto startShowcaseSprite = ButtonSprite::create("Start Showcase", "bigFont.fnt", "GJ_button_01.png", 0.7f);
    auto startShowcaseBtn = CCMenuItemSpriteExtra::create(
        startShowcaseSprite,
        this,
        menu_selector(MacroTab::onStartShowcase)
    );
    startShowcaseBtn->setPosition({size.width - 60.f, rowY2});
    m_menu->addChild(startShowcaseBtn);

    this->schedule(schedule_selector(MacroTab::updateStatus));

    return true;
}

void MacroTab::updateStatus(float) {
    auto mgr = BotManager::get();
    std::string text;
    if (mgr->isRecording()) {
        text = "Recording... frame " + std::to_string(mgr->currentFrame());
    } else if (mgr->isPlaying()) {
        text = "Playing... frame " + std::to_string(mgr->currentFrame());
    } else if (mgr->hasLoadedMacro()) {
        text = "Loaded: " + mgr->loadedMacroName();
    } else {
        text = "Idle";
    }
    m_statusLabel->setString(text.c_str());
}

void MacroTab::onRecordToggle(CCObject*) {
    auto mgr = BotManager::get();
    if (mgr->isRecording()) {
        mgr->stopRecording();
    } else {
        mgr->startRecording();
    }
}

void MacroTab::onPlayToggle(CCObject*) {
    auto mgr = BotManager::get();
    if (mgr->isPlaying()) {
        mgr->stopPlayback();
    } else {
        mgr->startPlayback();
    }
}

void MacroTab::onSave(CCObject*) {
    SaveMacroPopup::create([](std::string name) {
        auto result = BotManager::get()->saveRecording(name);
        if (!result) {
            Notification::create(result.unwrapErr(), NotificationIcon::Error)->show();
        } else {
            Notification::create("Macro saved", NotificationIcon::Success)->show();
        }
    })->show();
}

void MacroTab::onShowcaseToggle(CCObject*) {
}

void MacroTab::onStartShowcase(CCObject*) {
    auto mgr = BotManager::get();
    if (!mgr->hasLoadedMacro()) {
        Notification::create("Load a macro first", NotificationIcon::Warning)->show();
        return;
    }

    ShowcaseConfig config;
    config.enabled = m_showcaseToggle->isToggled();
    config.attemptCount = std::max(1, geode::utils::numFromString<int>(m_attemptsInput->getString()).unwrapOr(3));
    config.minProgress = geode::utils::numFromString<float>(m_minProgressInput->getString()).unwrapOr(5.f);
    config.maxProgress = geode::utils::numFromString<float>(m_maxProgressInput->getString()).unwrapOr(90.f);

    auto seedStr = m_seedInput->getString();
    if (!seedStr.empty()) {
        auto parsed = geode::utils::numFromString<uint32_t>(seedStr);
        if (parsed.isOk()) config.seed = parsed.unwrap();
    }

    mgr->configureShowcase(config);
    mgr->startShowcaseSequence();
}

}
