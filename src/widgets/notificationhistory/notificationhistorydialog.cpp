/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorydialog.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);

    mNotificationHistoryWidget->setObjectName(u"mNotificationHistoryWidget"_s);
    mainLayout->addWidget(mNotificationHistoryWidget);
    connect(mNotificationHistoryWidget, &NotificationHistoryWidget::showNotifyMessage, this, &NotificationHistoryDialog::showNotifyMessage);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &NotificationHistoryDialog::reject);
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
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myNotificationHistoryDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void NotificationHistoryDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myNotificationHistoryDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void NotificationHistoryDialog::addServerList(const QList<AccountManager::AccountDisplayInfo> &infos)
{
    mNotificationHistoryWidget->addServerList(infos);
}

#include "moc_notificationhistorydialog.cpp"
