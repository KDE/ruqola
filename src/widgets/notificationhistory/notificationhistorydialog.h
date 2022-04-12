/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

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

Q_SIGNALS:
    void openMessage(const QString &accountName, const QString &messageId, const QString &roomId);

private:
    NotificationHistoryWidget *const mNotificationHistoryWidget;
    void readConfig();
    void writeConfig();
};
