#pragma once

#include "MacroTypes.hpp"
#include <algorithm>

namespace showcasekit {

class MacroData {
protected:
    std::vector<MacroEvent> m_events;
    MacroMetadata m_metadata;
    bool m_sorted = true;

public:
    void clear() {
        m_events.clear();
        m_metadata = MacroMetadata();
        m_sorted = true;
    }

    void addEvent(MacroEvent const& event) {
        if (!m_events.empty() && event.frame < m_events.back().frame) {
            m_sorted = false;
        }
        m_events.push_back(event);
    }

    void sortIfNeeded() {
        if (!m_sorted) {
            std::stable_sort(m_events.begin(), m_events.end());
            m_sorted = true;
        }
    }

    std::vector<MacroEvent> const& getEvents() const {
        return m_events;
    }

    std::vector<MacroEvent>& getEventsMut() {
        return m_events;
    }

    size_t eventCount() const {
        return m_events.size();
    }

    bool isEmpty() const {
        return m_events.empty();
    }

    uint32_t lastFrame() const {
        if (m_events.empty()) return 0;
        return m_events.back().frame;
    }

    MacroMetadata& metadata() {
        return m_metadata;
    }

    MacroMetadata const& metadata() const {
        return m_metadata;
    }

    HeldButtonState heldStateAtFrame(uint32_t frame) const {
        HeldButtonState state;
        for (auto const& ev : m_events) {
            if (ev.frame > frame) break;
            bool* target = resolveTarget(state, ev.button, ev.player2);
            if (target) *target = ev.push;
        }
        return state;
    }

    size_t firstIndexAfter(uint32_t frame) const {
        size_t idx = 0;
        for (; idx < m_events.size(); ++idx) {
            if (m_events[idx].frame > frame) break;
        }
        return idx;
    }

    static bool* resolveTarget(HeldButtonState& state, MacroButton button, bool player2) {
        switch (button) {
            case MacroButton::Jump: return player2 ? &state.jump2 : &state.jump1;
            case MacroButton::Left: return player2 ? &state.left2 : &state.left1;
            case MacroButton::Right: return player2 ? &state.right2 : &state.right1;
        }
        return nullptr;
    }
};

}
