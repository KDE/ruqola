/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionui.h"
#include "ruqola_autogenerateui_debug.h"

#include <QWidget>

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUi::AutoGenerateInteractionUi(QObject *parent)
    : QObject(parent)
{
}

AutoGenerateInteractionUi::~AutoGenerateInteractionUi() = default;

QJsonObject AutoGenerateInteractionUi::generateJson() const
{
    return {};
}

bool AutoGenerateInteractionUi::parseInteractionUi(const QJsonObject &json)
{
    mTypeUi = convertTypeUiFromString(json["type"_L1].toString());
    if (mTypeUi == AutoGenerateInteractionUi::TypeUi::Unknown) {
        return false;
    }
    mTriggerId = json["triggerId"_L1].toString().toLatin1();
    mAppId = json["appId"_L1].toString().toLatin1();

    if (!mView) {
        mView = new AutoGenerateInteractionUiView(this);
    }
    mView->parseView(json["view"_L1].toObject());
    return true;
}

AutoGenerateInteractionUi::TypeUi AutoGenerateInteractionUi::convertTypeUiFromString(const QString &str) const
{
    if (str == "modal.update"_L1) {
        return AutoGenerateInteractionUi::TypeUi::ModalDialogUpdate;
    } else if (str == "modal.open"_L1) {
        return AutoGenerateInteractionUi::TypeUi::ModalDialogOpen;
    } else if (str == "contextual_bar.open"_L1) {
        return AutoGenerateInteractionUi::TypeUi::ContextualBarOpen;
    } else if (str == "contextual_bar.update"_L1) {
        return AutoGenerateInteractionUi::TypeUi::ContextualBarUpdate;
    } else {
        qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "Unknown TypeUi type " << str;
    }
    return AutoGenerateInteractionUi::TypeUi::Unknown;
}

AutoGenerateInteractionUiView *AutoGenerateInteractionUi::view() const
{
    return mView;
}

void AutoGenerateInteractionUi::setView(AutoGenerateInteractionUiView *newView)
{
    mView = newView;
}

QWidget *AutoGenerateInteractionUi::generateWidget(QWidget *parent)
{
    auto widget = new QWidget(parent);
    mView->generateWidget(widget);
    return widget;
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
    if (t.view()) {
        d.space() << "view" << *t.view();
    }
    return d;
}

#include "moc_autogenerateinteractionui.cpp"
