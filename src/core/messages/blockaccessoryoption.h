/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT BlockAccessoryOption
{
public:
    BlockAccessoryOption();
    ~BlockAccessoryOption();
    [[nodiscard]] QString value() const;
    void setValue(const QString &newValue);

    [[nodiscard]] bool operator==(const BlockAccessoryOption &other) const;

private:
    QString mValue;
};

LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const BlockAccessoryOption &t);
Q_DECLARE_TYPEINFO(BlockAccessoryOption, Q_RELOCATABLE_TYPE);
