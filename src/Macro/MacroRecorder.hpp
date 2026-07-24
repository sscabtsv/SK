#pragma once

#include "MacroData.hpp"

namespace showcasekit {

class MacroRecorder {
protected:
    MacroData m_data;
    bool m_recording = false;
    HeldButtonState m_liveState;

public:
    void start(std::string const& levelName);
    void stop();
    void reset();
    void onButton(MacroButton button, bool player2, bool push, uint32_t frame);

    bool isRecording() const { return m_recording; }
    MacroData const& data() const { return m_data; }
    MacroData& dataMut() { return m_data; }
    uint32_t frameCount() const { return m_data.lastFrame(); }
};

}
