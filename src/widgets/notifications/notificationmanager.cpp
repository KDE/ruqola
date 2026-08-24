/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "notificationmanager.h"
#include "ruqola_notification_manager_widget_debug.h"
#include "ruqolaglobalconfig.h"
#include <KActionCollection>
#include <KLocalizedString>
#include <KStandardActions>
#include <QApplication>
#include <QMenu>

#include <utility>

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

void NotificationManager::createSystemTray(QObject *parent)
{
#if !defined(Q_OS_IOS)
    if (!RuqolaGlobalConfig::self()->enableSystemTray()) {
        delete mNotification;
        mNotification = nullptr;
        mContextStatusMenu = nullptr;
        return;
    }
    if (!mNotification) {
        mNotification = new Notification(parent);
        auto trayMenu = mNotification->contextMenu();

        mContextStatusMenu = trayMenu->addMenu(i18nc("@item:inmenu Instant message presence status", "Status"));
        mContextStatusMenu->menuAction()->setVisible(false);
        trayMenu->addAction(mActionCollection->action(KStandardActions::name(KStandardActions::Preferences)));
        trayMenu->addAction(mActionCollection->action(KStandardActions::name(KStandardActions::ConfigureNotifications)));
        createSystrayToolTip();
    }
#endif
}

void NotificationManager::roomNeedAttention()
{
    Q_EMIT alert();
}

void NotificationManager::logout(const QString &accountName)
{
    mListTrayIcon.remove(accountName);
    createSystrayToolTip();
}

void NotificationManager::updateNotification(bool hasAlert, int nbUnread, const QString &accountName)
{
    qCDebug(RUQOLA_NOTIFICATION_MANAGER_WIDGETS_LOG) << " hasAlert " << hasAlert << " unreadNumber " << nbUnread << " account" << accountName;
    const Notification::TrayInfo info(nbUnread, hasAlert);
    if (info.hasNotification()) {
        mListTrayIcon.insert(accountName, info);
    } else {
        mListTrayIcon.remove(accountName);
    }
    createSystrayToolTip();
}

bool NotificationManager::notificationActivated() const
{
    return mNotification != nullptr;
}

void NotificationManager::createSystrayToolTip()
{
    QString str;
    bool hasAlert = false;
    int unreadMessage = 0;
    for (const auto &[key, value] : std::as_const(mListTrayIcon).asKeyValueRange()) {
        const Notification::TrayInfo &trayInfo = value;
        if (trayInfo.hasAlert) {
            hasAlert = true;
        }
        if (trayInfo.unreadMessage != 0) {
            if (mNotification) {
                if (!str.isEmpty()) {
                    str += u'\n';
                }
                str += i18np("%1 has %2 unread message", "%1 has %2 unread messages", key, trayInfo.unreadMessage);
            }
            unreadMessage += trayInfo.unreadMessage;
        }
    }
    qGuiApp->setBadgeNumber(unreadMessage);
    if (mNotification) {
        mNotification->updateToolTip(str, hasAlert);
    }
}

#include "moc_notificationmanager.cpp"
