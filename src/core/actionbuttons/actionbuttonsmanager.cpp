/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "actionbuttonsmanager.h"
#include "apps/appupdateinfojob.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqola_action_buttons_debug.h"
#include <QJsonArray>

ActionButtonsManager::ActionButtonsManager(RocketChatAccount *account, QObject *parent)
    : QObject{parent}
    , mRocketChatAccount(account)
{
}

ActionButtonsManager::~ActionButtonsManager() = default;

void ActionButtonsManager::fetchActionButtons()
{
    if (mRocketChatAccount) {
        auto job = new RocketChatRestApi::AppUpdateInfoJob(this);
        RocketChatRestApi::AppUpdateInfoJob::AppUpdateInfo info;
        info.mAppInfoType = RocketChatRestApi::AppUpdateInfoJob::AppInfoType::Apps;
        info.mAppMode = RocketChatRestApi::AppUpdateInfoJob::AppMode::Get;
        info.mAppInfoType = RocketChatRestApi::AppUpdateInfoJob::AppInfoType::ActionButton;
        job->setAppUpdateInfo(info);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::AppUpdateInfoJob::fetchActionButtonsDone, this, &ActionButtonsManager::parseActionButtons);
        if (!job->start()) {
            qCWarning(RUQOLA_ACTION_BUTTONS_LOG) << "Impossible to start AppUpdateInfoJob";
        }
    }
}

QList<ActionButton> ActionButtonsManager::actionButtons() const
{
    return mActionButtons;
}

void ActionButtonsManager::setActionButtons(const QList<ActionButton> &newActionButtons)
{
    if (mActionButtons != newActionButtons) {
        mActionButtons = newActionButtons;
        Q_EMIT actionButtonsChanged();
    }
}

void ActionButtonsManager::parseActionButtons(const QJsonArray &array)
{
    qDebug() << " void ActionButtonsManager::parseActionButtons(const QJsonArray &array) " << array;
    QList<ActionButton> buttons;
    for (const auto &r : array) {
        ActionButton act;
        act.parseActionButton(r.toObject());
        buttons.append(std::move(act));
    }
    setActionButtons(buttons);
}

QList<ActionButton> ActionButtonsManager::actionButtonsFromFilterActionInfo(const ActionButton::FilterActionInfo &filterInfo) const
{
    // qDebug() << " mActionButtons *********************** " << mActionButtons;
    QList<ActionButton> lists;
    for (const auto &act : mActionButtons) {
        if (act.category() == filterInfo.category) {
            if (act.buttonContext() == filterInfo.buttonContext) {
                if (act.buttonContext() == ActionButton::ButtonContext::MessageAction) {
                    if ((act.messageActionContexts() == ActionButton::MessageActionContext::Unknown)
                        || (act.messageActionContexts() & filterInfo.messageActionContext)) {
                        lists.append(act);
                    }
                } else if ((act.roomTypeFilters() == ActionButton::RoomTypeFilter::Unknown) || (act.roomTypeFilters() & filterInfo.roomTypeFilter)) {
                    lists.append(act);
                }
            }
        }
        // TODO check with hasOneRole
    }
    return lists;
}

#include "moc_actionbuttonsmanager.cpp"
