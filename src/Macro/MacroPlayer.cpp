#include "MacroPlayer.hpp"

namespace showcasekit {

void MacroPlayer::begin(MacroData const& data, uint32_t startFrame) {
    m_data = &data;
    m_startFrame = startFrame;
    m_elapsed = 0;
    m_finished = false;
    m_playing = true;
    m_cursor = data.firstIndexAfter(startFrame);

    if (!m_applyFn) return;

    HeldButtonState held = data.heldStateAtFrame(startFrame);
    if (held.jump1) m_applyFn(MacroButton::Jump, false, true);
    if (held.left1) m_applyFn(MacroButton::Left, false, true);
    if (held.right1) m_applyFn(MacroButton::Right, false, true);
    if (held.jump2) m_applyFn(MacroButton::Jump, true, true);
    if (held.left2) m_applyFn(MacroButton::Left, true, true);
    if (held.right2) m_applyFn(MacroButton::Right, true, true);
}

void MacroPlayer::step() {
    if (!m_playing || !m_data) return;

    uint32_t target = m_startFrame + m_elapsed;
    auto const& events = m_data->getEvents();

    while (m_cursor < events.size() && events[m_cursor].frame <= target) {
        auto const& ev = events[m_cursor];
        if (m_applyFn) m_applyFn(ev.button, ev.player2, ev.push);
        ++m_cursor;
    }

    if (m_cursor >= events.size() && !m_finished) {
        m_finished = true;
        m_playing = false;
        if (m_finishFn) m_finishFn();
        return;
    }

    ++m_elapsed;
}

void MacroPlayer::stop() {
    m_playing = false;
    m_data = nullptr;
    m_cursor = 0;
    m_elapsed = 0;
}

}
