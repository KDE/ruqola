/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorydialog.h"
#include "notificationhistorywidget.h"

NotificationHistoryDialog::NotificationHistoryDialog(QWidget *parent)
    : QDialog(parent)
    , mNotificationHistoryWidget(new NotificationHistoryWidget(this))
{
}

NotificationHistoryDialog::~NotificationHistoryDialog()
{
}
