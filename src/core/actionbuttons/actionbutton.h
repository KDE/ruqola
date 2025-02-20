/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
class LIBRUQOLACORE_EXPORT ActionButton
{
public:
    ActionButton();
    ~ActionButton();

    [[nodiscard]] QString appId() const;
    void setAppId(const QString &newAppId);

    [[nodiscard]] QString actionId() const;
    void setActionId(const QString &newActionId);

private:
    QString mAppId;
    QString mActionId;
};

Q_DECLARE_METATYPE(ActionButton)
Q_DECLARE_TYPEINFO(ActionButton, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ActionButton &t);
