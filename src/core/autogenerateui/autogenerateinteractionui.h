/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT AutoGenerateInteractionUi
{
    Q_GADGET
public:
    enum class TypeUi : uint8_t {
        Unknown,
        ModalDialog,
        ContextualBar,
    };
    Q_ENUM(TypeUi)

    AutoGenerateInteractionUi();
    ~AutoGenerateInteractionUi();
    void parseInteractionUi(const QJsonObject &json);

    [[nodiscard]] QByteArray appId() const;
    void setAppId(const QByteArray &newAppId);

    [[nodiscard]] TypeUi typeUi() const;
    void setTypeUi(TypeUi newTypeUi);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUi &other) const;

    [[nodiscard]] QByteArray triggerId() const;
    void setTriggerId(const QByteArray &newTriggerId);

private:
    QByteArray mAppId;
    QByteArray mTriggerId;
    TypeUi mTypeUi = TypeUi::Unknown;
};
Q_DECLARE_METATYPE(AutoGenerateInteractionUi)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUi, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUi &t);
