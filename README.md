
# ruqola

Ruqola is a Rocket.Chat client for the KDE desktop.

## Features

It supports multi-account, search in room, open close rooms, direct message, thread, discussions, teams, administrator.
RC settings can be changed directly.

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

## ASAN

We can use sanitizers.supp file
example: LSAN_OPTIONS=suppressions=../sanitizers.supp ./bin/ruqolaserverconfigtest

## Maintainer

- Laurent Montel <montel@kde.org>

