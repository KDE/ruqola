
# ruqola

Ruqola is a Rocket.Chat client for the KDE desktop.

## Features

It supports RC feature:
- Direct Message
- group channel
- channel
- Support autotranslate (when RC has it)
- Configuring room notification
- Configuring room
- Thread message support
- Discussion room
- Teams room support
- Configuring own account
- Registering new account
- Support multi account
- Search message in room.
- Show mention/attachment/Pinned message/start message
- Support emoji
- Video support
- Configure administrator support (add/remove roles, add/remove users, add/remove permissions, configure rooms etc.) 
- Support OTR (in progress)
- support gif image
- Add support for uploading attachment files.
- Show unread message information
- Allow to block/unblock users

Extra features:
- Allow to reply directly to message from notification (kde features)
- Dnd image from ruqola to website or local folder
- Store message in local database so we can search in offline



## Source Code

You can get ruqola from:

- <https://github.com/KDE/ruqola>
- git@invent.kde.org:network/ruqola.git 
- https://invent.kde.org/network/ruqola.git

## Requirements

`cmake`, `cmake-extra-tools`, `qt5-websockets`, `qt5-networkauth`, `qt5-multimedia`, `kwidgetaddons`, `ki18n`, `kcrash`, `kcoreaddons`, `syntaxhighlighting`, `sonnet`, `textwidgets`, `notifyconfig`, `kio`, `iconthemes`, `xmlgui`

## How to build ruqola

````bash
mkdir build
cd build
cmake ..
make
make install
````
or using CMakePresets support (need cmake > 3.20)
````
cmake --preset dev && cmake --build --preset dev
````

## ASAN

We can use sanitizers.supp file
example: LSAN_OPTIONS=suppressions=../sanitizers.supp ./bin/ruqolaserverconfigtest

## Maintainer

- Laurent Montel <montel@kde.org>

