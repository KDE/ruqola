/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
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

    [[nodiscard]] QString url() const;
    void setUrl(const QString &newUrl);

    [[nodiscard]] static QJsonObject serialize(const BlockAction &block);
    [[nodiscard]] static BlockAction deserialize(const QJsonObject &o);

private:
    QString mActionId;
    QString mText;
    QString mType;
    QString mBlockId;
    QString mValue;
    QString mUrl;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const BlockAction &t);
class QDebug;
Q_DECLARE_TYPEINFO(BlockAction, Q_RELOCATABLE_TYPE);
