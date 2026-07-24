#pragma once

#include <Geode/Result.hpp>
#include "../Macro/MacroRecorder.hpp"
#include "../Macro/MacroPlayer.hpp"
#include "../Macro/MacroData.hpp"
#include "../Macro/ShowcaseAttempts.hpp"
#include <string>
#include <optional>

namespace showcasekit {

enum class PendingAction {
    None,
    RecordOnNextLevel,
    PlayOnPress
};

class BotManager {
protected:
    BotManager() = default;

    MacroRecorder m_recorder;
    MacroPlayer m_player;
    MacroData m_loadedMacro;
    ShowcaseAttempts m_showcase;

    uint32_t m_frame = 0;
    float m_stepAccumulator = 0.f;
    static constexpr float kFixedStep = 1.f / 240.f;

    bool m_simulating = false;
    bool m_hasLoadedMacro = false;
    bool m_showcaseSequenceActive = false;
    std::string m_loadedMacroName;
    std::string m_currentLevelName;

public:
    static BotManager* get();

    void init();

    void onLevelInit(std::string const& levelName);
    void onLevelReset();
    void onUpdate(float dt);
    void onRealButton(MacroButton button, bool player2, bool push);

    void startRecording();
    void stopRecording();
    geode::Result<> saveRecording(std::string const& name);

    geode::Result<> loadMacro(std::string const& name);
    bool hasLoadedMacro() const { return m_hasLoadedMacro; }
    std::string const& loadedMacroName() const { return m_loadedMacroName; }

    void startPlayback();
    void stopPlayback();

    void configureShowcase(ShowcaseConfig const& config);
    void startShowcaseSequence();

    bool isRecording() const { return m_recorder.isRecording(); }
    bool isPlaying() const { return m_player.isPlaying(); }
    uint32_t currentFrame() const { return m_frame; }

    void applyToPlayers(MacroButton button, bool player2, bool push);
};

}
