/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QDebug>
class LIBRUQOLACORE_TESTS_EXPORT MessageStarred
{
public:
    MessageStarred();
    [[nodiscard]] bool isStarred() const;
    void setIsStarred(bool isStarred);

    [[nodiscard]] bool operator==(MessageStarred other) const;
    void parse(const QJsonObject &o);

private:
    bool mIsStarred = false;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, MessageStarred t);
