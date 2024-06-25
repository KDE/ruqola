/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "textconverter.h"
#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>

class EmojiManager;
class Message;
class MessageCache;

namespace TextConverterCMark
{
extern "C" {
[[nodiscard]] LIBRUQOLACORE_EXPORT char *convertMessageTextCMark(const TextConverter::ConvertMessageTextSettings &settings);
}
[[nodiscard]] LIBRUQOLACORE_EXPORT QString convertMessageText(const TextConverter::ConvertMessageTextSettings &settings,
                                                              QByteArray &needUpdateMessageId,
                                                              int &recusiveIndex);
}
