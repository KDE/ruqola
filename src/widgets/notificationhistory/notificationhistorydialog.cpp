/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorydialog.h"
#include "notificationhistorywidget.h"

#include <KLocalizedString>

#include <QDialogButtonBox>
#include <QVBoxLayout>

NotificationHistoryDialog::NotificationHistoryDialog(QWidget *parent)
    : QDialog(parent)
    , mNotificationHistoryWidget(new NotificationHistoryWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Notification"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mNotificationHistoryWidget->setObjectName(QStringLiteral("mNotificationHistoryWidget"));
    mainLayout->addWidget(mNotificationHistoryWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &NotificationHistoryDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &NotificationHistoryDialog::accept);
}

NotificationHistoryDialog::~NotificationHistoryDialog()
{
}
