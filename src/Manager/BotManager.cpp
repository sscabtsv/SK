#include "BotManager.hpp"
#include "ButtonMap.hpp"
#include "../Macro/MacroFile.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace showcasekit {

BotManager* BotManager::get() {
    static BotManager instance;
    return &instance;
}

void BotManager::init() {
    MacroFile::macroDirectory();

    m_player.setApplyFn([this](MacroButton button, bool player2, bool push) {
        applyToPlayers(button, player2, push);
    });

    m_player.setFinishFn([this]() {
    });
}

void BotManager::onLevelInit(std::string const& levelName) {
    m_currentLevelName = levelName;
    m_frame = 0;
    m_stepAccumulator = 0.f;
}

void BotManager::onLevelReset() {
    m_frame = 0;
    m_stepAccumulator = 0.f;
    m_player.stop();

    if (m_showcaseSequenceActive) {
        if (!m_showcase.isFinalAttempt()) {
            m_player.begin(m_loadedMacro, 0);
        } else {
            m_player.begin(m_loadedMacro, 0);
            m_showcaseSequenceActive = false;
        }
    }
}

void BotManager::onUpdate(float dt) {
    if (dt <= 0.f || dt > 0.5f) return;

    m_stepAccumulator += dt;
    while (m_stepAccumulator >= kFixedStep) {
        m_stepAccumulator -= kFixedStep;
        ++m_frame;

        if (m_player.isPlaying()) {
            m_player.step();
        }

        if (m_showcaseSequenceActive && m_player.isPlaying() && !m_showcase.isFinalAttempt()) {
            auto stop = m_showcase.currentStopPoint();
            if (stop.has_value()) {
                auto pl = PlayLayer::get();
                if (pl) {
                    float percent = pl->getCurrentPercent();
                    if (percent >= *stop) {
                        m_showcase.advanceAttempt();
                        if (pl->m_player1) {
                            m_simulating = true;
                            pl->m_player1->playerDestroyed(true);
                            m_simulating = false;
                        }
                    }
                }
            }
        }
    }
}

void BotManager::onRealButton(MacroButton button, bool player2, bool push) {
    if (m_simulating) return;
    m_recorder.onButton(button, player2, push, m_frame);
}

void BotManager::startRecording() {
    m_player.stop();
    m_recorder.start(m_currentLevelName);
}

void BotManager::stopRecording() {
    m_recorder.stop();
}

Result<> BotManager::saveRecording(std::string const& name) {
    if (m_recorder.data().isEmpty()) {
        return Err("Nothing recorded yet");
    }
    auto data = m_recorder.data();
    data.metadata().name = name;
    auto path = MacroFile::macroDirectory() / (name + ".gdr");
    return MacroFile::save(data, path);
}

Result<> BotManager::loadMacro(std::string const& name) {
    auto path = MacroFile::macroDirectory() / (name + ".gdr");
    auto result = MacroFile::load(path);
    if (!result) {
        return Err(result.unwrapErr());
    }
    m_loadedMacro = result.unwrap();
    m_loadedMacroName = name;
    m_hasLoadedMacro = true;
    return Ok();
}

void BotManager::startPlayback() {
    if (!m_hasLoadedMacro) return;
    m_recorder.stop();
    m_player.begin(m_loadedMacro, m_frame);
}

void BotManager::stopPlayback() {
    m_player.stop();
    m_showcaseSequenceActive = false;
}

void BotManager::configureShowcase(ShowcaseConfig const& config) {
    m_showcase.configure(config);
}

void BotManager::startShowcaseSequence() {
    if (!m_hasLoadedMacro) return;
    m_showcase.reset();
    m_showcaseSequenceActive = true;

    auto pl = PlayLayer::get();
    if (pl) {
        pl->resetLevel();
    } else {
        m_player.begin(m_loadedMacro, 0);
    }
}

void BotManager::applyToPlayers(MacroButton button, bool player2, bool push) {
    auto pl = PlayLayer::get();
    if (!pl) return;

    auto player = player2 ? pl->m_player2 : pl->m_player1;
    if (!player) return;

    m_simulating = true;
    if (push) {
        player->pushButton(toPlayerButton(button));
    } else {
        player->releaseButton(toPlayerButton(button));
    }
    m_simulating = false;
}

}
