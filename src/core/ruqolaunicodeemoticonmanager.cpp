/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "ruqolaunicodeemoticonmanager.h"
#include <TextEmoticonsCore/UnicodeEmoticonManager>

using namespace Qt::Literals::StringLiterals;
RuqolaUnicodeEmoticonManager::RuqolaUnicodeEmoticonManager(QObject *parent)
    : QObject{parent}
    , mUnicodeEmoticonManager(new TextEmoticonsCore::UnicodeEmoticonManager(u":/emoji.json"_s, this))
{
}

RuqolaUnicodeEmoticonManager::~RuqolaUnicodeEmoticonManager() = default;

RuqolaUnicodeEmoticonManager *RuqolaUnicodeEmoticonManager::self()
{
    static RuqolaUnicodeEmoticonManager s_self;
    return &s_self;
}
