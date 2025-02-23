/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionui.h"
#include "ruqola_action_buttons_debug.h"
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUi::AutoGenerateInteractionUi() = default;

AutoGenerateInteractionUi::~AutoGenerateInteractionUi() = default;

void AutoGenerateInteractionUi::parseInteractionUi(const QJsonObject &json)
{
    mTriggerId = json["triggerId"_L1].toString().toLatin1();
    mAppId = json["appId"_L1].toString().toLatin1();
    mTypeUi = convertTypeUiFromString(json["type"_L1].toString());
}

AutoGenerateInteractionUi::TypeUi AutoGenerateInteractionUi::convertTypeUiFromString(const QString &str) const
{
    if (str == "modal.open"_L1) {
        return AutoGenerateInteractionUi::TypeUi::ModalDialog;
    } else if (str == "contextual_bar.open"_L1) {
        return AutoGenerateInteractionUi::TypeUi::ContextualBar;
    } else {
        qCWarning(RUQOLA_ACTION_BUTTONS_LOG) << "Unknown TypeUi type " << str;
    }
    return AutoGenerateInteractionUi::TypeUi::Unknown;
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

#include "moc_autogenerateinteractionui.cpp"
