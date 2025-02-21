/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionui.h"

AutoGenerateInteractionUi::AutoGenerateInteractionUi() = default;

AutoGenerateInteractionUi::~AutoGenerateInteractionUi() = default;

void AutoGenerateInteractionUi::parseInteractionUi(const QJsonObject &json)
{
    // TODO
}

QString AutoGenerateInteractionUi::appId() const
{
    return mAppId;
}

void AutoGenerateInteractionUi::setAppId(const QString &newAppId)
{
    mAppId = newAppId;
}
