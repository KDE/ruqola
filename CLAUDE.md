# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working
with code in this repository.

## Project

Ruqola is a [Rocket.Chat](https://www.rocket.chat/) desktop client for
KDE, built on Qt 6 and KDE Frameworks 6. Maintainer: Laurent Montel.

## Build, test, run

The canonical build uses CMake presets (defined in `CMakePresets.json`).
Builds go into `build-<presetName>/` next to the source tree:

```bash
cmake --preset dev && cmake --build --preset dev  # Debug + compile_commands
cmake --preset dev-mold                           # Debug + mold linker
cmake --preset asan                               # address+undefined sanitizers
cmake --preset unity                              # CMake UNITY (faster compile)
cmake --preset release                            # Release, no tests
cmake --preset clazy                              # clazy static analysis
cmake --preset ftime-trace                        # clang -ftime-trace builds
```

Tests are CTest-driven; sources live next to the code in `autotests/`
subdirectories (one per library). Run all tests for a preset:

```bash
ctest --preset dev
ctest --preset dev -R <pattern>     # filter by test name
ctest --preset dev -V               # verbose
```

A single autotest binary can also be run directly from `build-dev/bin/`
(e.g. `./build-dev/bin/accountmanagertest`). The `tests/` directory at
the top level contains interactive GUI test programs (not CTest tests).

Install prefix defaults to `$KF6`. ASAN suppressions:
`LSAN_OPTIONS=suppressions=../sanitizers.supp ./bin/<test>`.

## Pre-commit hooks

`pre-commit` is auto-installed by CMake when configuring from a git
checkout (it looks for `prek` first, then `pre-commit`). The project
ships its own clang-format config used by the hook:
`.clang-format-pre-commit` (the hook is invoked with
`--style=file:.clang-format-pre-commit`, not the editor-facing
`.clang-format`). Other hooks: codespell, gersemi (CMake formatter),
markdownlint, shfmt, shellcheck, ruff.

If hooks are missing, `pre-commit install -f` (see
`README-pre-commit.md`).

## Architecture

The codebase is split into layered libraries under `src/`, plus plugins
and the application executable:

- **`src/rocketchatrestapi-qt/`** → `librocketchatrestapi-qt` — REST API
  client. One `QObject`-based job class per Rocket.Chat REST endpoint
  (grouped into subdirs like `channels/`, `users/`, `rooms/`, `chat/`,
  `e2e/`, `subscriptions/`…). Knows nothing about Ruqola's domain
  model.
- **`src/core/`** → `libruqolacore` — domain layer: account state,
  models (`model/`), local SQLite database (`localdatabase/`),
  DDP/WebSocket client (`ddpapi/`), authentication
  (`authenticationmanager/` with DDP and REST variants), caches, emoji,
  OTR/E2E, etc. Central type is `RocketChatAccount` (one per server).
  Process-wide singleton is `Ruqola` (`ruqola.h`), which owns the
  `AccountManager`.
- **`src/widgets/`** → `libruqolawidgets` — QWidget-based UI: main
  window, room/channel views, dialogs (`administratordialog/`,
  `configuredialog/`, …), delegates, etc.
- **`src/plugins/`** — runtime-loaded plugins:
  - `authentication/` (password, google, github, gitlab, facebook,
    twitter, personalaccesstoken)
  - `textplugin/` (aitext, sharetext, webshortcut)
  - `toolsplugin/` (autogeneratetext, grabscreen, aiactions)
- **`src/apps/`** → the `ruqola` executable (`main.cpp`) and the
  `e2ekeytool` helper.

Optional integrations are gated by `config-ruqola.h.in` macros,
controlled by CMake options in the top-level `CMakeLists.txt`:
`USE_DBUS` (Linux default; falls back to `KDSingleApplication`
otherwise), `OPTION_USE_E2E_SUPPORT` (requires OpenSSL),
`OPTION_USE_PLASMA_ACTIVITIES`, `OPTION_ADD_AUTOGENERATETEXT`,
`OPTION_ADD_OFFLINE_SUPPORT`, `HAVE_KUSERFEEDBACK`, `HAVE_NETWORKMANAGER`,
and several `KF6Text*` (ktextaddons) features. Code must
`#include "config-ruqola.h"` and `#if`-guard usage of these.

Network transport: the live connection uses **DDP over WebSocket**
(`src/core/ddpapi/`, on top of `abstractwebsocket`/`ruqolawebsocket`)
for realtime events; one-off operations use the REST API library. A
`RocketChatBackend` per account wires both together.

## Conventions

- C++ style: WebKit base with project tweaks (see `.clang-format`) —
  4-space indent, 160-column lines, Linux braces. `clang-tidy` config in
  `.clang-tidy` (most `bugprone-*`, `performance-*`, `readability-*`
  enabled with documented exceptions).
- `-DQT_NO_CONTEXTLESS_CONNECT` is set globally — all `QObject::connect`
  calls must pass a context object.
- Clang builds add `-Wshadow` (not enabled for GCC; see CMakeLists
  comment).
- Logging categories are exported via `ECMQtDeclareLoggingCategory` to
  `ruqola.categories`.

## Release procedure

See `README-RELEASE.md`: bump `RUQOLA_RELEASE_VERSION` and flip
`RUQOLA_STABLE_VERSION` in the top-level `CMakeLists.txt`, update
`src/widgets/needupdateversion/needupdateversionutils.cpp`, and update
versions in `sysadmin/repo-metadata` and `sysadmin/ci-utilities`.
