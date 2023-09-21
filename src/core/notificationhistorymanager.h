/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "notificationinfo.h"
#include <QObject>
class NotificationHistoryModel;

class LIBRUQOLACORE_EXPORT NotificationHistoryManager : public QObject
{
    Q_OBJECT
public:
    explicit NotificationHistoryManager(QObject *parent = nullptr);
    ~NotificationHistoryManager() override;

    static NotificationHistoryManager *self();

    [[nodiscard]] NotificationHistoryModel *notificationHistoryModel() const;

    void addNotification(const NotificationInfo &info);

Q_SIGNALS:
    void newNotification();

private:
    NotificationHistoryModel *const mNotificationHistoryModel;
};
