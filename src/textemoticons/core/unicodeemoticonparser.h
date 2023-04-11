/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "unicodeemoticon.h"
#include <QList>
namespace TextEmoticonsCore
{
class UnicodeEmoticonParser
{
public:
    UnicodeEmoticonParser();
    ~UnicodeEmoticonParser();

    Q_REQUIRED_RESULT QList<UnicodeEmoticon> parse(const QJsonObject &o) const;
    Q_REQUIRED_RESULT static int changeOrder(const QString &name);
};
}
