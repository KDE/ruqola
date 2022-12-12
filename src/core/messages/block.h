/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QDebug>
#include <QJsonObject>
class LIBRUQOLACORE_TESTS_EXPORT Block
{
public:
    Block();
    Q_REQUIRED_RESULT bool operator==(const Block &other) const;

    static QJsonObject serialize(const Block &block);
    static Block fromJSon(const QJsonObject &o);
    void parseBlock(const QJsonObject &block);
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Block &t);
Q_DECLARE_TYPEINFO(Block, Q_MOVABLE_TYPE);
