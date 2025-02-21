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
        ModalDialog,
        ContextualBar,
    };
    Q_ENUM(TypeUi)

    AutoGenerateInteractionUi();
    ~AutoGenerateInteractionUi();
    void parseInteractionUi(const QJsonObject &json);

    [[nodiscard]] QString appId() const;
    void setAppId(const QString &newAppId);

private:
    QString mAppId;
};
