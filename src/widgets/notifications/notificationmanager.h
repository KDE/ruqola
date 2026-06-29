/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
class Notification;
class QMenu;
class KActionCollection;
class LIBRUQOLACORE_EXPORT NotificationManager : public QObject
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
Q_SIGNALS:
    void alert();

private:
    KActionCollection *const mActionCollection;
    Notification *mNotification = nullptr;
    QMenu *mContextStatusMenu = nullptr;
};
