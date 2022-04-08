/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorymanager.h"
#include "model/notificationhistorymodel.h"

NotificationHistoryManager::NotificationHistoryManager(QObject *parent)
    : QObject{parent}
    , mNotificationHistoryModel(new NotificationHistoryModel(this))
{
}

NotificationHistoryManager::~NotificationHistoryManager()
{
}

NotificationHistoryManager *NotificationHistoryManager::self()
{
    static NotificationHistoryManager s_self;
    return &s_self;
}

NotificationHistoryModel *NotificationHistoryManager::notificationHistoryModel() const
{
    return mNotificationHistoryModel;
}

void NotificationHistoryManager::addNotification(const NotificationInfo &info)
{
    mNotificationHistoryModel->addNotification(info);
    Q_EMIT newNotification();
}
