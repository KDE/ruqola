/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonunicodeutils.h"
using namespace TextEmoticonsCore;
QString EmoticonUnicodeUtils::emojiFontName()
{
#ifdef Q_OS_WIN
    return QStringLiteral("Segoe UI Emoji");
#else
    return QStringLiteral("NotoColorEmoji");
#endif
}

QString EmoticonUnicodeUtils::recentIdentifier()
{
    return QStringLiteral("recents");
}

QString EmoticonUnicodeUtils::customIdentifier()
{
    return QStringLiteral("customs");
}
