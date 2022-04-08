/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorydialog.h"
#include "notificationhistorywidget.h"

#include <KLocalizedString>

#include <KConfigGroup>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>

namespace
{
const char myNotificationHistoryDialogConfigGroupName[] = "NotificationHistoryDialog";
}

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
    readConfig();
}

NotificationHistoryDialog::~NotificationHistoryDialog()
{
    writeConfig();
}

void NotificationHistoryDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myNotificationHistoryDialogConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void NotificationHistoryDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myNotificationHistoryDialogConfigGroupName);
    group.writeEntry("Size", size());
}
