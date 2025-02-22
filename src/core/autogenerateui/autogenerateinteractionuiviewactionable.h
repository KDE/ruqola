/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QJsonObject>

class LIBRUQOLACORE_TESTS_EXPORT AutoGenerateInteractionUiViewActionable
{
public:
    AutoGenerateInteractionUiViewActionable();
    ~AutoGenerateInteractionUiViewActionable();
    void parse(const QJsonObject &json);

    [[nodiscard]] QString appId() const;
    void setAppId(const QString &newAppId);

    [[nodiscard]] QString blockId() const;
    void setBlockId(const QString &newBlockId);

    [[nodiscard]] QString actionId() const;
    void setActionId(const QString &newActionId);

private:
    QString mAppId;
    QString mBlockId;
    QString mActionId;
};
