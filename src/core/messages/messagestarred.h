/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QDebug>
class LIBRUQOLACORE_TESTS_EXPORT MessageStarred
{
public:
    MessageStarred();
    Q_REQUIRED_RESULT bool isStarred() const;
    void setIsStarred(bool isStarred);

    Q_REQUIRED_RESULT bool operator==(MessageStarred other) const;
    void parse(const QJsonObject &o);

private:
    bool mIsStarred = false;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, MessageStarred t);
