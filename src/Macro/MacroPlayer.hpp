#pragma once

#include "MacroData.hpp"
#include <functional>

namespace showcasekit {

using MacroApplyFn = std::function<void(MacroButton, bool, bool)>;
using MacroFinishFn = std::function<void()>;

class MacroPlayer {
protected:
    MacroData const* m_data = nullptr;
    bool m_playing = false;
    bool m_finished = false;
    uint32_t m_startFrame = 0;
    uint32_t m_elapsed = 0;
    size_t m_cursor = 0;
    MacroApplyFn m_applyFn;
    MacroFinishFn m_finishFn;

public:
    void setApplyFn(MacroApplyFn fn) { m_applyFn = std::move(fn); }
    void setFinishFn(MacroFinishFn fn) { m_finishFn = std::move(fn); }

    void begin(MacroData const& data, uint32_t startFrame = 0);
    void step();
    void stop();

    bool isPlaying() const { return m_playing; }
    bool isFinished() const { return m_finished; }
    uint32_t elapsedFrames() const { return m_elapsed; }
    uint32_t absoluteFrame() const { return m_startFrame + m_elapsed; }
};

}
