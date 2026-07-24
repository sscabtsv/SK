#include "MacroRecorder.hpp"

namespace showcasekit {

void MacroRecorder::start(std::string const& levelName) {
    m_data.clear();
    m_data.metadata().levelName = levelName;
    m_liveState = HeldButtonState();
    m_recording = true;
}

void MacroRecorder::stop() {
    m_recording = false;
    m_data.sortIfNeeded();
}

void MacroRecorder::reset() {
    m_recording = false;
    m_data.clear();
    m_liveState = HeldButtonState();
}

void MacroRecorder::onButton(MacroButton button, bool player2, bool push, uint32_t frame) {
    if (!m_recording) return;

    bool* target = MacroData::resolveTarget(m_liveState, button, player2);
    if (target) {
        if (*target == push) return;
        *target = push;
    }

    MacroEvent ev;
    ev.frame = frame;
    ev.button = button;
    ev.player2 = player2;
    ev.push = push;
    m_data.addEvent(ev);
}

}
