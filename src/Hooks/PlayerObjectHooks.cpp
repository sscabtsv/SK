#include <Geode/modify/PlayerObject.hpp>
#include "../Manager/BotManager.hpp"
#include "../Manager/ButtonMap.hpp"

using namespace geode::prelude;
using namespace showcasekit;

class $modify(SKPlayerObjectHook, PlayerObject) {
    void pushButton(PlayerButton button) {
        auto pl = PlayLayer::get();
        bool isP2 = pl && this == pl->m_player2;
        PlayerObject::pushButton(button);
        BotManager::get()->onRealButton(fromPlayerButton(button), isP2, true);
    }

    void releaseButton(PlayerButton button) {
        auto pl = PlayLayer::get();
        bool isP2 = pl && this == pl->m_player2;
        PlayerObject::releaseButton(button);
        BotManager::get()->onRealButton(fromPlayerButton(button), isP2, false);
    }
};
