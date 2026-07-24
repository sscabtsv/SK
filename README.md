# ShowcaseKit

A Geode mod for Geometry Dash that records and replays showcase runs of your own levels. Targets Geode 5.3.0 / GD 2.2081, Android32 and Android64 only.

## Status — Phase 1 (this delivery)

Implemented and wired end-to-end:

- Frame-accurate input capture (press/release, both players) via `PlayerObject::pushButton`/`releaseButton` hooks
- Fixed-timestep frame counter (240 Hz accumulator) driving both recording and playback
- `.gdr` binary macro format (custom, versioned) with save/load
- Mid-level playback synchronization: starting playback partway through an attempt skips completed inputs, re-enters any button that was mid-hold at the sync frame, and continues from the next event
- Showcase Attempts: configurable attempt count, min/max progress range, optional seed, stratified random stop points so attempts don't cluster, automatic re-attempt sequencing, final attempt plays through in full
- Pause menu integration opening a dark-themed popup GUI with the Macro tab: record/play toggles, save (named), load via a scrollable dropdown of saved `.gdr` files, live status, and the Showcase Attempts panel
- Original logo and pause-menu icon (`logo.png`, `resources/SK_icon.png`)
- Android32/Android64-only CMake guard and CI workflow

## Not yet built (next phases, by your priority)

Speedhack, Frame Stepper, Noclip, Show Trajectory, Show Hitboxes, and their corresponding UI tabs are not in this delivery — they layer on top of this same `BotManager`/hook architecture next.

## Before you build

I don't have network or SDK access in the environment that generated this, so none of this has been compiled against the real Geode 5.3.0 headers/bindings. Things worth double-checking against your local bindings before your first build:

- `PlayLayer::init(GJGameLevel*, bool, bool)` signature and `GJGameLevel::m_levelName` field name
- `PlayerObject::pushButton`/`releaseButton` signatures and the `PlayerButton` enum values
- `GJBaseGameLayer::m_player1`/`m_player2` member names (accessed via `PlayLayer`)
- `PlayLayer::getCurrentPercent()` and `PlayerObject::playerDestroyed(bool)` signatures
- `geode::ScrollLayer` and `geode::TextInput` API shape (`m_contentLayer`, `setCommonFilter`, `CommonFilter` enum) for your Geode 5.3.0 install
- `geode::Result<T>` accessor names (`unwrap`/`unwrapErr` vs whatever 5.3.0 uses)
- `CircleButtonSprite::createWithSpriteFrameName` parameter order/overload (used for the pause menu button)

Given you already forked `geode-sdk/bindings` for RipBot on 2.2081, you'll likely hit the same handful of binding gaps here — the architecture is written so any renamed member/function is a one-line fix, not a redesign.

## Build

```
git clone <this repo>
cd ShowcaseKit
export GEODE_SDK=/path/to/geode-sdk
cmake -B build -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=armeabi-v7a
cmake --build build
```

Or push to GitHub — `.github/workflows/build.yml` builds Android32 and Android64 via `geode-sdk/build-geode-mod`.

## Layout

```
src/
  Macro/     engine-agnostic recording/playback/serialization/showcase logic
  Manager/   BotManager — the only place that talks to both Macro/ and the GD engine
  Hooks/     $modify hooks into PlayerObject, PlayLayer, PauseLayer
  UI/        popup, tabs, dropdown, save-name popup, theme
resources/   spritesheet inputs (icon)
logo.png     mod list icon
```
