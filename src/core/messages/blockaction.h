/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QDebug>

class LIBRUQOLACORE_EXPORT BlockAction
{
public:
    BlockAction();
    [[nodiscard]] QString actionId() const;
    void setActionId(const QString &newActionId);

    [[nodiscard]] bool operator==(const BlockAction &other) const;

    [[nodiscard]] QString text() const;
    void setText(const QString &newText);

    [[nodiscard]] QString type() const;
    void setType(const QString &newType);

private:
    QString mActionId;
    QString mText;
    QString mType;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const BlockAction &t);
Q_DECLARE_TYPEINFO(BlockAction, Q_RELOCATABLE_TYPE);
