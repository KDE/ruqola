/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_export.h"
#include <QDialog>
class NotificationHistoryWidget;
class LIBRUQOLAWIDGETS_EXPORT NotificationHistoryDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NotificationHistoryDialog(QWidget *parent = nullptr);
    ~NotificationHistoryDialog() override;

    void addServerList(const QStringList &serverNames);

Q_SIGNALS:
    void showNotifyMessage(const QString &accountName, const QString &messageId, const QString &roomId);

private:
    NotificationHistoryWidget *const mNotificationHistoryWidget;
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
};
