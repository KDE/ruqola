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
    virtual ~AutoGenerateInteractionUiViewActionable();
    virtual void parse(const QJsonObject &json);

    [[nodiscard]] QByteArray appId() const;
    void setAppId(const QByteArray &newAppId);

    [[nodiscard]] QByteArray blockId() const;
    void setBlockId(const QByteArray &newBlockId);

    [[nodiscard]] QByteArray actionId() const;
    void setActionId(const QByteArray &newActionId);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewActionable &other) const;

private:
    QByteArray mAppId;
    QByteArray mBlockId;
    QByteArray mActionId;
};
