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

void ActionButtonsGenerator::clearActionButtons()
{
    // Check list of apps action
    qDeleteAll(mListActionButton);
    mListActionButton.clear();
}

void ActionButtonsGenerator::generateActionButtons(const QList<ActionButton> &actionButtons, QMenu *menu, const QByteArray &roomId)
{
    clearActionButtons();
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
