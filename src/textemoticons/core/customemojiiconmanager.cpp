/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customemojiiconmanager.h"

using namespace TextEmoticonsCore;
CustomEmojiIconManager::CustomEmojiIconManager(QObject *parent)
    : QObject(parent)
{
}

CustomEmojiIconManager::~CustomEmojiIconManager()
{
}

QIcon CustomEmojiIconManager::generateIcon(const QString &customIdentifier)
{
    Q_UNUSED(customIdentifier)
    return {};
}
