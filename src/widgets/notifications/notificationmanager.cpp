/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "notificationmanager.h"
#include "notifications/notification.h"
#include "ruqolaglobalconfig.h"
#include <KActionCollection>
#include <KLocalizedString>
#include <KStandardActions>
#include <QApplication>
#include <QMenu>

NotificationManager::NotificationManager(KActionCollection *actionCollection, QObject *parent)
    : QObject(parent)
    , mActionCollection(actionCollection)
{
}

NotificationManager::~NotificationManager() = default;

QMenu *NotificationManager::contextStatusMenu() const
{
    return mContextStatusMenu;
}

void NotificationManager::createSystemTray()
{
#if !defined(Q_OS_IOS)
    if (!RuqolaGlobalConfig::self()->enableSystemTray()) {
        delete mNotification;
        mNotification = nullptr;
        mContextStatusMenu = nullptr;
        return;
    }
    if (!mNotification) {
        mNotification = new Notification(this);
        auto trayMenu = mNotification->contextMenu();

        mContextStatusMenu = mNotification->contextMenu()->addMenu(i18nc("@item:inmenu Instant message presence status", "Status"));
        mContextStatusMenu->menuAction()->setVisible(false);
        trayMenu->addAction(mActionCollection->action(KStandardActions::name(KStandardActions::Preferences)));
        trayMenu->addAction(mActionCollection->action(KStandardActions::name(KStandardActions::ConfigureNotifications)));
        // Create systray to show notifications on Desktop
        connect(mNotification, &Notification::alert, this, &NotificationManager::alert);
    }
#endif
}

void NotificationManager::roomNeedAttention()
{
    if (mNotification) {
        mNotification->roomNeedAttention();
    }
}

void NotificationManager::logout(const QString &accountName)
{
    if (mNotification) {
        mNotification->clearNotification(accountName);
    }
}

void NotificationManager::updateNotification(bool hasAlert, int nbUnread, const QString &accountName)
{
    if (mNotification) {
        mNotification->updateNotification(hasAlert, nbUnread, accountName);
    }
}

bool NotificationManager::notificationActivated() const
{
    return mNotification != nullptr;
}
#include "moc_notificationmanager.cpp"
