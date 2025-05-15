/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "actionbuttonsutils.h"
#include "actionbuttons/actionbuttonutil.h"
#include "autogenerateui/autogenerateinteractionutil.h"
#include "misc/appsuiinteractionjob.h"

#include <QLocale>
#include <QMenu>
#include <QUuid>
/*
QList<QAction *> ActionButtonsUtils::generateActionButtons(const QList<ActionButton> &actionButtons, QMenu *menu, QWidget *widget)
{
    QList<QAction *> mListActionButton;
    const QString lang = QLocale().name();
    auto actSeparator = new QAction(widget);
    actSeparator->setSeparator(true);
    mListActionButton.append(actSeparator);
    menu->addAction(actSeparator);
    for (const auto &actionButton : actionButtons) {
        auto act = new QAction(widget);
        const QString translateIdentifier = ActionButtonUtil::generateTranslateIdentifier(actionButton);
        const QString appId = QString::fromLatin1(actionButton.appId());
        act->setText(mCurrentRocketChatAccount->getTranslatedIdentifier(lang, translateIdentifier));
        const QByteArray roomId = mRoom->roomId();
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
    return mListActionButton;
}
*/
