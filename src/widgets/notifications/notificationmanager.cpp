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

#if HAVE_UNITY_SUPPORT
#include "unityservicemanager.h"
#endif

NotificationManager::NotificationManager(KActionCollection *actionCollection, QObject *parent)
    : QObject(parent)
    , mActionCollection(actionCollection)
{
}

NotificationManager::~NotificationManager() = default;

void NotificationManager::updateUnityService([[maybe_unused]] int unreadMessage)
{
#if HAVE_UNITY_SUPPORT
    unityServiceManager()->setCount(unreadMessage);
#endif
}

#if HAVE_UNITY_SUPPORT
UnityServiceManager *NotificationManager::unityServiceManager()
{
    if (!mUnityServiceManager) {
        mUnityServiceManager = new UnityServiceManager(this);
    }
    return mUnityServiceManager;
}
#endif

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
    for (const auto &[key, value] : mListTrayIcon.asKeyValueRange()) {
        const Notification::TrayInfo trayInfo = value;
        if (mNotification) {
            if (trayInfo.hasAlert) {
                hasAlert = trayInfo.hasAlert;
            }
        }
        if (trayInfo.unreadMessage != 0) {
            if (mNotification) {
                if (!str.isEmpty()) {
                    str += u'\n';
                }
                str += i18n("%1 has %2 Unread Message", key, trayInfo.unreadMessage);
            }
            unreadMessage += trayInfo.unreadMessage;
        }
    }
    updateUnityService(unreadMessage);
    if (mNotification) {
        mNotification->updateToolTip(str, hasAlert);
    }
}

#include "moc_notificationmanager.cpp"
