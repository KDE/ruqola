/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "unicodeemoticon.h"
#include <QVector>

class LIBRUQOLACORE_TESTS_EXPORT UnicodeEmoticonParser
{
public:
    UnicodeEmoticonParser();
    ~UnicodeEmoticonParser();

    Q_REQUIRED_RESULT QVector<UnicodeEmoticon> parse(const QJsonObject &o) const;
    Q_REQUIRED_RESULT static int changeOrder(const QString &name);
};
