/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "accountmanager.h"
#include "libruqolawidgets_export.h"
#include <QDialog>
class NotificationHistoryWidget;
class LIBRUQOLAWIDGETS_EXPORT NotificationHistoryDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NotificationHistoryDialog(QWidget *parent = nullptr);
    ~NotificationHistoryDialog() override;

    void addServerList(const QList<AccountManager::AccountDisplayInfo> &info);

Q_SIGNALS:
    void showNotifyMessage(const QString &accountName, const QByteArray &messageId, const QByteArray &roomId);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    NotificationHistoryWidget *const mNotificationHistoryWidget;
};
