/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright 2018 Laurent Montel <montel@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "notification.h"
#include "ruqola.h"
#include "ruqola_debug.h"
#ifdef UNITY_SUPPORT
#include "unityservicemanager.h"
#endif

#include <KLocalizedString>
#include <QIcon>

Notification::Notification(QObject *parent)
    : KStatusNotifierItem(parent)
{
    createTrayIcon();
}

Notification::~Notification()
{
#ifdef UNITY_SUPPORT
    delete mUnityServiceManager;
#endif
}

void Notification::createTrayIcon()
{
    setToolTipTitle(QStringLiteral("Ruqola"));
    setIconByPixmap(QIcon(QStringLiteral(":/icons/systray.png")));
}

void Notification::clearNotification(const QString &account)
{
    mListTrayIcon.remove(account);
    createToolTip();
}

void Notification::updateNotification(bool hasAlert, int unreadNumber, const QString &account)
{
    qCDebug(RUQOLA_LOG) << " hasAlert " << hasAlert << " unreadNumber " << unreadNumber << " account" << account;
    const TrayInfo info(unreadNumber, hasAlert);
    if (info.hasNotification()) {
        mListTrayIcon.insert(account, info);
    } else {
        mListTrayIcon.remove(account);
    }
    createToolTip();
}

void Notification::createToolTip()
{
    QMapIterator<QString, TrayInfo> i(mListTrayIcon);
    QString str;
    bool firstElement = true;
    bool hasAlert = false;
    int unreadMessage = 0;
    while (i.hasNext()) {
        i.next();
        if (firstElement && !str.isEmpty()) {
            firstElement = false;
            str += QLatin1Char('\n');
        }
        if (!i.key().isEmpty()) {
            str += i.key() + QLatin1Char('\n');
        }
        const TrayInfo trayInfo = i.value();
        if (trayInfo.hasAlert) {
            hasAlert = trayInfo.hasAlert;
        }
        if (trayInfo.unreadMessage != 0) {
            str += i18n("Has %1 Unread Message", trayInfo.unreadMessage);
            unreadMessage += trayInfo.unreadMessage;
        }
    }
    setToolTipSubTitle(str);
    updateUnityService(unreadMessage);
    if (status() == KStatusNotifierItem::Passive && (!str.isEmpty() || hasAlert)) {
        setStatus(KStatusNotifierItem::Active);
    } else if (status() == KStatusNotifierItem::Active && (str.isEmpty() && !hasAlert)) {
        setStatus(KStatusNotifierItem::Passive);
    }
}

void Notification::updateUnityService(int unreadMessage)
{
#ifdef UNITY_SUPPORT
    unityServiceManager()->setCount(unreadMessage);
#else
    Q_UNUSED(unreadMessage);
#endif
}

#ifdef UNITY_SUPPORT
UnityServiceManager *Notification::unityServiceManager()
{
    if (!mUnityServiceManager) {
        mUnityServiceManager = new UnityServiceManager();
    }
    return mUnityServiceManager;
}

#endif
