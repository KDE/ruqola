/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include "libruqolacore_export.h"
#include <KStatusNotifierItem>
#include <QMap>

class UnityServiceManager;
class LIBRUQOLACORE_EXPORT Notification : public KStatusNotifierItem
{
    Q_OBJECT
public:
    struct TrayInfo {
        TrayInfo(int unread, bool alert)
            : unreadMessage(unread)
            , hasAlert(alert)
        {
        }

        [[nodiscard]] bool hasNotification() const
        {
            return (unreadMessage != 0) || hasAlert;
        }

        int unreadMessage = 0;
        bool hasAlert = false;
    };

    explicit Notification(QObject *parent = nullptr);
    ~Notification() override;

    void roomNeedAttention();

    void updateToolTip(const QString &str, bool hasAlert);

Q_SIGNALS:
    void alert();

private:
    /**
     * @brief Creates tray icon consisting of actions
     */
    LIBRUQOLACORE_NO_EXPORT void createTrayIcon();
};
