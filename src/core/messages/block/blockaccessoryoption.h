/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT BlockAccessoryOption
{
public:
    BlockAccessoryOption();
    ~BlockAccessoryOption();
    [[nodiscard]] QString value() const;
    void setValue(const QString &newValue);

    [[nodiscard]] bool operator==(const BlockAccessoryOption &other) const;

    [[nodiscard]] QString text() const;
    void setText(const QString &newText);

    void parse(const QJsonObject &obj);

    [[nodiscard]] static QJsonObject serialize(const BlockAccessoryOption &block);
    [[nodiscard]] static BlockAccessoryOption deserialize(const QJsonObject &o);

private:
    QString mText;
    QString mValue;
};
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const BlockAccessoryOption &t);
Q_DECLARE_TYPEINFO(BlockAccessoryOption, Q_RELOCATABLE_TYPE);
