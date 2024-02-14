/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorydialog.h"
#include "notificationhistorywidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

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
    connect(mNotificationHistoryWidget, &NotificationHistoryWidget::showNotifyMessage, this, &NotificationHistoryDialog::showNotifyMessage);

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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myNotificationHistoryDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void NotificationHistoryDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myNotificationHistoryDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void NotificationHistoryDialog::addServerList(const QList<AccountManager::AccountDisplayInfo> &infos)
{
    mNotificationHistoryWidget->addServerList(infos);
}

#include "moc_notificationhistorydialog.cpp"
