/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionui.h"
#include "connection.h"
#include "misc/appsuiinteractionjob.h"
#include "rocketchataccount.h"
#include "ruqola_autogenerateui_debug.h"

#include <QWidget>

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUi::AutoGenerateInteractionUi(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mRocketChatAccount(account)
{
}

AutoGenerateInteractionUi::~AutoGenerateInteractionUi() = default;

QJsonObject AutoGenerateInteractionUi::generateJson() const
{
    QJsonObject o;
    if (mView) {
        o["view"_L1] = mView->serialize();
    }
    return o;
}

bool AutoGenerateInteractionUi::parseInteractionUi(const QJsonObject &json)
{
    if (mView) {
        mView->clear();
        mView->deleteLater();
        mView = nullptr;
    }
    mTypeUi = convertTypeUiFromString(json["type"_L1].toString());
    if (mTypeUi == AutoGenerateInteractionUi::TypeUi::Unknown) {
        return false;
    }
    mTriggerId = json["triggerId"_L1].toString().toLatin1();
    mAppId = json["appId"_L1].toString().toLatin1();

    if (!mView) {
        mView = new AutoGenerateInteractionUiView(this);
        connect(mView, &AutoGenerateInteractionUiView::actionChanged, this, &AutoGenerateInteractionUi::slotActionChanged);
        connect(mView, &AutoGenerateInteractionUiView::closeButtonClicked, this, &AutoGenerateInteractionUi::slotCloseButtonClicked);
        connect(mView, &AutoGenerateInteractionUiView::submitButtonClicked, this, &AutoGenerateInteractionUi::slotSubmitButtonClicked);
    }
    mView->parseView(json["view"_L1].toObject());
    return true;
}

void AutoGenerateInteractionUi::slotActionChanged(const AutoGenerateInteractionUtil::ViewBlockActionUserInfo &info)
{
    qDebug() << " AutoGenerateInteractionUi::slotActionChanged : " << info;
    qDebug() << " this " << *this;
    if (mRocketChatAccount) {
        // Use AppId
        // TODO send message
    }
}

void AutoGenerateInteractionUi::slotCloseButtonClicked()
{
    qDebug() << " void AutoGenerateInteractionUi::slotCloseButtonClicked()";
    if (mRocketChatAccount) {
        auto job = new RocketChatRestApi::AppsUiInteractionJob(this);
        RocketChatRestApi::AppsUiInteractionJob::AppsUiInteractionJobInfo info;
        /*
        info.methodName = appId;
        AutoGenerateInteractionUtil::ViewBlockActionUserInfo actionButtonInfo;
        actionButtonInfo.actionId = actionButton.actionId();
        actionButtonInfo.triggerId = QUuid::createUuid().toByteArray(QUuid::Id128);
        actionButtonInfo.roomId = roomId;
        info.messageObj = AutoGenerateInteractionUtil::createViewClosedUser(actionButtonInfo);
        */
        job->setAppsUiInteractionJobInfo(info);

        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::AppsUiInteractionJob::appsUiInteractionDone, this, [this](const QJsonObject &replyObject) {
            // TODO close if necessary
            // Q_EMIT uiInteractionRequested(replyObject);
        });
        if (!job->start()) {
            qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "Impossible to start AppsUiInteractionJob job";
        }
        // Use AppId
        // TODO send message
    }
}

void AutoGenerateInteractionUi::slotSubmitButtonClicked()
{
    qDebug() << " void AutoGenerateInteractionUi::slotSubmitButtonClicked()";
    if (mRocketChatAccount) {
        // Use AppId
        // TODO send message
    }
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
    mView->generateWidget(widget, mRocketChatAccount);
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
