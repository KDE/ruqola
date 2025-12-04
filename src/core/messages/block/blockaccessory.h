/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "blockaccessoryoption.h"
#include "libruqolacore_export.h"
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT BlockAccessory
{
    Q_GADGET
public:
    enum class AccessoryType : uint8_t {
        Unknown,
        Button,
        Overflow,
        // TODO add more
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

    [[nodiscard]] static QJsonObject serialize(const BlockAccessory &block);
    [[nodiscard]] static BlockAccessory deserialize(const QJsonObject &o);

    [[nodiscard]] bool isValid() const;
    [[nodiscard]] BlockAccessory::AccessoryType type() const;
    void setType(BlockAccessory::AccessoryType newType);

    [[nodiscard]] QList<BlockAccessoryOption> options() const;
    void setOptions(const QList<BlockAccessoryOption> &newOptions);

    [[nodiscard]] QString text() const;
    void setText(const QString &newText);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static QString convertEnumToStr(AccessoryType newBlockType);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static BlockAccessory::AccessoryType convertAccessoryTypeToEnum(const QString &type);

    QByteArray mActionId;
    QString mValue;
    QString mText;
    BlockAccessory::AccessoryType mType = BlockAccessory::AccessoryType::Unknown;
    QList<BlockAccessoryOption> mOptions;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const BlockAccessory &t);
class QDebug;
Q_DECLARE_TYPEINFO(BlockAccessory, Q_RELOCATABLE_TYPE);
