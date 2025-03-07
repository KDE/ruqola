/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT BlockAccessory
{
    Q_GADGET
public:
    enum class AccessoryType : uint8_t {
        Unknown,
        Button,
    };
    Q_ENUM(AccessoryType)

    BlockAccessory();
    ~BlockAccessory();

    [[nodiscard]] QByteArray actionId() const;
    void setActionId(const QByteArray &newActionId);

    [[nodiscard]] QString value() const;
    void setValue(const QString &newValue);

    [[nodiscard]] bool operator==(const BlockAccessory &other) const;

    void parseAccessory(const QJsonObject &o);

private:
    QByteArray mActionId;
    QString mValue;
    // TODO Options
    // Type
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const BlockAccessory &t);
Q_DECLARE_TYPEINFO(BlockAccessory, Q_RELOCATABLE_TYPE);
