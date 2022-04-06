/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QObject>
class NotificationHistoryModel;

class LIBRUQOLACORE_EXPORT NotificationHistoryManager : public QObject
{
    Q_OBJECT
public:
    explicit NotificationHistoryManager(QObject *parent = nullptr);
    ~NotificationHistoryManager() override;

    static NotificationHistoryManager *self();

    Q_REQUIRED_RESULT NotificationHistoryModel *notificationHistoryModel() const;

private:
    NotificationHistoryModel *const mNotificationHistoryModel;
};
