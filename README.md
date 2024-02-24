# Ruqola

Ruqola is a [Rocket.Chat](https://www.rocket.chat/) client for the KDE desktop.

![Ruqola Main Window](https://cdn.kde.org/screenshots/ruqola/ruqola.png)

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
- Auto-away
- Two-factor authentication via TOTP
- Two-factor authentication via email
- Administrator support (Server Info, Rooms, Custom User Status, Custom Sounds, Custom Emoji, Users, Invites, View Log, Permissions, Roles, Oauth), Administrator settings (Message, Account, Retention Policy, File Upload, Encryption)
- Export Messages
- Console Moderation
- Implement Message Url Preview
- Channel List style
- Add Forward Message support

Extra features:
- Allow to reply directly to message from notification (kde features)
- Dnd image from ruqola to website or local folder
- Store message in local database so we can search in offline
- Mark All Channels are read
- Add notification history.
- Add import export Accounts.
- Implement message layout (normal/compact)
- Increase/Decrease global font by using CTRL++/CTRL+- shortcut


## Source Code

You can get ruqola from:

- <https://github.com/KDE/ruqola>
- git clone git@invent.kde.org:network/ruqola.git 
- git clone https://invent.kde.org/network/ruqola.git

## Requirements

`cmake`, `cmake-extra-tools`, `qt-websockets`, `qt-networkauth`, `qt-multimedia`, `kwidgetaddons`, `ki18n`, `kcrash`, `kcoreaddons`, `syntaxhighlighting`, `sonnet`, `textwidgets`, `notifyconfig`, `kio`, `iconthemes`, `xmlgui`, `ktextaddons`, `qtkeychain`

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


Build against qt6
````
cmake --preset dev-qt6 && cmake --build --preset dev-qt6
````


## ASAN

We can use sanitizers.supp file
example: LSAN_OPTIONS=suppressions=../sanitizers.supp ./bin/ruqolaserverconfigtest


## UNITY

We can build with cmake unity support
````
cmake --preset unity && cmake --build --preset unity
````

## Maintainer

- Laurent Montel <montel@kde.org>

