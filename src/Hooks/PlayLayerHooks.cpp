#include <Geode/modify/PlayLayer.hpp>
#include "../Manager/BotManager.hpp"

using namespace geode::prelude;
using namespace showcasekit;

class $modify(SKPlayLayerHook, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }

        std::string levelName = level ? std::string(level->m_levelName) : std::string("Unknown");
        BotManager::get()->onLevelInit(levelName);
        return true;
    }

    void update(float dt) {
        PlayLayer::update(dt);
        BotManager::get()->onUpdate(dt);
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        BotManager::get()->onLevelReset();
    }
};
