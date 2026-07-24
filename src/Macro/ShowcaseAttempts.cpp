#include "ShowcaseAttempts.hpp"
#include <algorithm>

namespace showcasekit {

void ShowcaseAttempts::configure(ShowcaseConfig const& config) {
    m_config = config;
    m_stopPoints.clear();
    m_currentAttempt = 0;

    if (!m_config.enabled || m_config.attemptCount <= 0) return;

    float lo = std::min(m_config.minProgress, m_config.maxProgress);
    float hi = std::max(m_config.minProgress, m_config.maxProgress);
    hi = std::min(hi, 99.f);
    lo = std::max(lo, 0.f);
    if (hi <= lo) hi = lo + 1.f;

    std::mt19937 rng(m_config.seed.value_or(std::random_device{}()));

    float span = hi - lo;
    float binSize = span / static_cast<float>(m_config.attemptCount);

    for (int i = 0; i < m_config.attemptCount; ++i) {
        float binLo = lo + binSize * static_cast<float>(i);
        float binHi = binLo + binSize;
        float margin = binSize * 0.15f;
        std::uniform_real_distribution<float> dist(binLo + margin, std::max(binLo + margin, binHi - margin));
        m_stopPoints.push_back(dist(rng));
    }

    std::sort(m_stopPoints.begin(), m_stopPoints.end());
}

void ShowcaseAttempts::reset() {
    m_currentAttempt = 0;
}

std::optional<float> ShowcaseAttempts::currentStopPoint() const {
    if (!m_config.enabled) return std::nullopt;
    if (m_currentAttempt >= static_cast<int>(m_stopPoints.size())) return std::nullopt;
    return m_stopPoints[m_currentAttempt];
}

void ShowcaseAttempts::advanceAttempt() {
    ++m_currentAttempt;
}

}
