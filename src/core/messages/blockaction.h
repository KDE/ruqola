/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT BlockAction
{
public:
    BlockAction();

    void parseAction(const QJsonObject &o);

    [[nodiscard]] QString actionId() const;
    void setActionId(const QString &newActionId);

    [[nodiscard]] bool operator==(const BlockAction &other) const;

    [[nodiscard]] QString text() const;
    void setText(const QString &newText);

    [[nodiscard]] QString type() const;
    void setType(const QString &newType);

    [[nodiscard]] QString blockId() const;
    void setBlockId(const QString &newBlockId);

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] QString value() const;
    void setValue(const QString &newValue);

private:
    QString mActionId;
    QString mText;
    QString mType;
    QString mBlockId;
    QString mValue;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const BlockAction &t);
Q_DECLARE_TYPEINFO(BlockAction, Q_RELOCATABLE_TYPE);
