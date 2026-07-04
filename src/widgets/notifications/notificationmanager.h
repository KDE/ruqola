/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "config-ruqola.h"
#include "libruqolawidgets_private_export.h"
#include "notifications/notification.h"
class Notification;
class QMenu;
class KActionCollection;
#if HAVE_UNITY_SUPPORT
class UnityServiceManager;
#endif
class LIBRUQOLAWIDGETS_TESTS_EXPORT NotificationManager : public QObject
{
    Q_OBJECT
public:
    explicit NotificationManager(KActionCollection *actionCollection, QObject *parent = nullptr);
    ~NotificationManager() override;

    [[nodiscard]] QMenu *contextStatusMenu() const;
    void createSystemTray();
    void roomNeedAttention();
    void logout(const QString &accountName);
    void updateNotification(bool hasAlert, int nbUnread, const QString &accountName);
    [[nodiscard]] bool notificationActivated() const;

#if HAVE_UNITY_SUPPORT
    [[nodiscard]] UnityServiceManager *unityServiceManager();
#endif
Q_SIGNALS:
    void alert();

private:
    LIBRUQOLACORE_NO_EXPORT void updateUnityService(int unreadMessage);
    LIBRUQOLACORE_NO_EXPORT void createSystrayToolTip();

    QMap<QString, Notification::TrayInfo> mListTrayIcon;
    KActionCollection *const mActionCollection;
    Notification *mNotification = nullptr;
    QMenu *mContextStatusMenu = nullptr;
#if HAVE_UNITY_SUPPORT
    UnityServiceManager *mUnityServiceManager = nullptr;
#endif
};
