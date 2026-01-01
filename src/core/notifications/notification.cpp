/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "notification.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_notification_debug.h"
#if HAVE_UNITY_SUPPORT
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
#if HAVE_UNITY_SUPPORT
    delete mUnityServiceManager;
#endif
}

void Notification::createTrayIcon()
{
    setToolTipTitle(i18n("Ruqola"));
    setIconByPixmap(QIcon(u":/icons/systray.png"_s));
    setCategory(KStatusNotifierItem::Communications);
}

void Notification::clearNotification(const QString &account)
{
    mListTrayIcon.remove(account);
    createToolTip();
}

void Notification::roomNeedAttention()
{
    qCDebug(RUQOLA_NOTIFICATION_LOG) << " emit alert";
    Q_EMIT alert();
}

void Notification::updateNotification(bool hasAlert, int unreadNumber, const QString &account)
{
    qCDebug(RUQOLA_NOTIFICATION_LOG) << " hasAlert " << hasAlert << " unreadNumber " << unreadNumber << " account" << account;
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
    QString str;
    bool firstElement = true;
    bool hasAlert = false;
    int unreadMessage = 0;
    for (const auto &[key, value] : mListTrayIcon.asKeyValueRange()) {
        if (firstElement && !str.isEmpty()) {
            firstElement = false;
            str += u'\n';
        }
        const TrayInfo trayInfo = value;
        if (trayInfo.hasAlert) {
            hasAlert = trayInfo.hasAlert;
        }
        if (trayInfo.unreadMessage != 0) {
            str += i18n("%1 has %2 Unread Message", key, trayInfo.unreadMessage);
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

void Notification::updateUnityService([[maybe_unused]] int unreadMessage)
{
#if HAVE_UNITY_SUPPORT
    unityServiceManager()->setCount(unreadMessage);
#endif
}

#if HAVE_UNITY_SUPPORT
UnityServiceManager *Notification::unityServiceManager()
{
    if (!mUnityServiceManager) {
        mUnityServiceManager = new UnityServiceManager();
    }
    return mUnityServiceManager;
}

#endif

#include "moc_notification.cpp"
