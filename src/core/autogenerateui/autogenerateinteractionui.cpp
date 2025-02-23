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

QByteArray AutoGenerateInteractionUi::appId() const
{
    return mAppId;
}

void AutoGenerateInteractionUi::setAppId(const QByteArray &newAppId)
{
    mAppId = newAppId;
}

AutoGenerateInteractionUi::TypeUi AutoGenerateInteractionUi::typeUi() const
{
    return mTypeUi;
}

void AutoGenerateInteractionUi::setTypeUi(TypeUi newTypeUi)
{
    mTypeUi = newTypeUi;
}

bool AutoGenerateInteractionUi::operator==(const AutoGenerateInteractionUi &other) const
{
    return other.appId() == appId() && other.typeUi() == typeUi() && other.triggerId() == triggerId();
}

QByteArray AutoGenerateInteractionUi::triggerId() const
{
    return mTriggerId;
}

void AutoGenerateInteractionUi::setTriggerId(const QByteArray &newTriggerId)
{
    mTriggerId = newTriggerId;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUi &t)
{
    d.space() << "appId:" << t.appId();
    d.space() << "triggerId:" << t.triggerId();
    d.space() << "typeUi:" << t.typeUi();
    return d;
}
