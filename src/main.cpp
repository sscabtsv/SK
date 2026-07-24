#include <Geode/Geode.hpp>
#include "Manager/BotManager.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    showcasekit::BotManager::get()->init();
}
