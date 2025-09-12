/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "actionbuttonsgenerator.h"
#include "actionbuttons/actionbuttonutil.h"
#include "autogenerateui/autogenerateinteractionutil.h"
#include "connection.h"
#include "misc/appsuiinteractionjob.h"
#include "ruqolawidgets_debug.h"

#include <QLocale>
#include <QMenu>
#include <QUuid>

ActionButtonsGenerator::ActionButtonsGenerator(QObject *parent)
    : QObject(parent)
{
}

ActionButtonsGenerator::~ActionButtonsGenerator() = default;

void ActionButtonsGenerator::generateMessageBoxActionButtons(const QList<ActionButton> &actionButtons, QMenu *menu, const QByteArray &roomId)
{
    clearActionButtons();
    if (actionButtons.isEmpty()) {
        return;
    }
    const QString lang = QLocale().name();
    auto actSeparator = new QAction(this);
    actSeparator->setSeparator(true);
    mListActionButton.append(actSeparator);
    menu->addAction(actSeparator);
    for (const auto &actionButton : actionButtons) {
        auto act = new QAction(this);
        const QString translateIdentifier = ActionButtonUtil::generateTranslateIdentifier(actionButton);
        const QString appId = QString::fromLatin1(actionButton.appId());
        act->setText(mCurrentRocketChatAccount->getTranslatedIdentifier(lang, translateIdentifier));
        connect(act, &QAction::triggered, this, [this, actionButton, appId, roomId]() {
            auto job = new RocketChatRestApi::AppsUiInteractionJob(this);
            RocketChatRestApi::AppsUiInteractionJob::AppsUiInteractionJobInfo info;
            info.methodName = appId;
            AutoGenerateInteractionUtil::ActionButtonInfo actionButtonInfo;
            actionButtonInfo.actionId = actionButton.actionId();
            actionButtonInfo.triggerId = QUuid::createUuid().toByteArray(QUuid::Id128);
            actionButtonInfo.roomId = roomId;
            info.messageObj = AutoGenerateInteractionUtil::createMessageBoxActionButton(actionButtonInfo);
            // qDebug() << " info " << info;
            job->setAppsUiInteractionJobInfo(info);

            mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::AppsUiInteractionJob::appsUiInteractionDone, this, [this](const QJsonObject &replyObject) {
                Q_EMIT uiInteractionRequested(replyObject);
            });
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AppsUiInteractionJob job";
            }
        });
        mListActionButton.append(act);
        menu->addAction(act);
    }
}

void ActionButtonsGenerator::clearActionButtons()
{
    // Check list of apps action
    qDeleteAll(mListActionButton);
    mListActionButton.clear();
}

void ActionButtonsGenerator::generateRoomActionButtons(const QList<ActionButton> &actionButtons, QMenu *menu, const QByteArray &roomId)
{
    clearActionButtons();
    if (actionButtons.isEmpty()) {
        return;
    }
    const QString lang = QLocale().name();
    auto actSeparator = new QAction(this);
    actSeparator->setSeparator(true);
    mListActionButton.append(actSeparator);
    menu->addAction(actSeparator);
    for (const auto &actionButton : actionButtons) {
        auto act = new QAction(this);
        const QString translateIdentifier = ActionButtonUtil::generateTranslateIdentifier(actionButton);
        const QString appId = QString::fromLatin1(actionButton.appId());
        act->setText(mCurrentRocketChatAccount->getTranslatedIdentifier(lang, translateIdentifier));
        connect(act, &QAction::triggered, this, [this, actionButton, appId, roomId]() {
            auto job = new RocketChatRestApi::AppsUiInteractionJob(this);
            RocketChatRestApi::AppsUiInteractionJob::AppsUiInteractionJobInfo info;
            info.methodName = appId;
            AutoGenerateInteractionUtil::ActionButtonInfo actionButtonInfo;
            actionButtonInfo.actionId = actionButton.actionId();
            actionButtonInfo.triggerId = QUuid::createUuid().toByteArray(QUuid::Id128);
            actionButtonInfo.roomId = roomId;
            info.messageObj = AutoGenerateInteractionUtil::createRoomActionButton(actionButtonInfo);
            // qDebug() << " info " << info;
            job->setAppsUiInteractionJobInfo(info);

            mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::AppsUiInteractionJob::appsUiInteractionDone, this, [this](const QJsonObject &replyObject) {
                Q_EMIT uiInteractionRequested(replyObject);
            });
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AppsUiInteractionJob job";
            }
        });
        mListActionButton.append(act);
        menu->addAction(act);
    }
}

void ActionButtonsGenerator::generateMessageActionButtons(const QList<ActionButton> &actionButtons,
                                                          QMenu *menu,
                                                          const QByteArray &roomId,
                                                          const QByteArray &messageId)
{
    clearActionButtons();
    if (actionButtons.isEmpty()) {
        return;
    }
    const QString lang = QLocale().name();
    auto actSeparator = new QAction(this);
    actSeparator->setSeparator(true);
    mListActionButton.append(actSeparator);
    menu->addAction(actSeparator);
    for (const auto &actionButton : actionButtons) {
        auto act = new QAction(this);
        const QString translateIdentifier = ActionButtonUtil::generateTranslateIdentifier(actionButton);
        const QString appId = QString::fromLatin1(actionButton.appId());
        act->setText(mCurrentRocketChatAccount->getTranslatedIdentifier(lang, translateIdentifier));
        connect(act, &QAction::triggered, this, [this, actionButton, appId, roomId, messageId]() {
            auto job = new RocketChatRestApi::AppsUiInteractionJob(this);
            RocketChatRestApi::AppsUiInteractionJob::AppsUiInteractionJobInfo info;
            info.methodName = appId;
            AutoGenerateInteractionUtil::ActionMessageInfo actionMessageInfo;
            actionMessageInfo.actionId = actionButton.actionId();
            actionMessageInfo.triggerId = QUuid::createUuid().toByteArray(QUuid::Id128);
            actionMessageInfo.roomId = roomId;
            actionMessageInfo.messageId = messageId;
            info.messageObj = AutoGenerateInteractionUtil::createMessageActionButton(actionMessageInfo);
            // qDebug() << " info " << info;
            job->setAppsUiInteractionJobInfo(info);

            mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::AppsUiInteractionJob::appsUiInteractionDone, this, [this](const QJsonObject &replyObject) {
                Q_EMIT uiInteractionRequested(replyObject);
            });
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AppsUiInteractionJob job";
            }
        });
        mListActionButton.append(act);
        menu->addAction(act);
    }
}

void ActionButtonsGenerator::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mCurrentRocketChatAccount = account;
}

#include "moc_actionbuttonsgenerator.cpp"
