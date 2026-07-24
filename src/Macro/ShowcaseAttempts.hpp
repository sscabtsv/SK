#pragma once

#include <vector>
#include <cstdint>
#include <optional>
#include <random>

namespace showcasekit {

struct ShowcaseConfig {
    bool enabled = false;
    int attemptCount = 3;
    float minProgress = 5.f;
    float maxProgress = 90.f;
    std::optional<uint32_t> seed;
};

class ShowcaseAttempts {
protected:
    ShowcaseConfig m_config;
    std::vector<float> m_stopPoints;
    int m_currentAttempt = 0;

public:
    void configure(ShowcaseConfig const& config);
    void reset();

    bool isActive() const { return m_config.enabled; }
    bool isFinalAttempt() const { return m_currentAttempt >= m_config.attemptCount; }
    int currentAttemptIndex() const { return m_currentAttempt; }
    int totalAttempts() const { return m_config.attemptCount; }

    std::optional<float> currentStopPoint() const;
    void advanceAttempt();
};

}
