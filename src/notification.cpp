/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
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
#include <KLocalizedString>

#include <QAction>
#include <QMenu>
#include <QMessageBox>

Notification::Notification(QObject *parent)
    : KStatusNotifierItem(parent)
{
    createTrayIcon();
}

void Notification::createTrayIcon()
{
    setToolTipTitle(QStringLiteral("Ruqola"));
    setIconByPixmap(QIcon(QStringLiteral(":/icons/systray.png")));
}

void Notification::updateNotification(bool hasAlert, int unreadNumber, const QString &account)
{
    //qCDebug(RUQOLA_LOG) << " hasAlert " << hasAlert << " unreadNumber " << unreadNumber << " account" << account;
    qDebug() << " hasAlert " << hasAlert << " unreadNumber " << unreadNumber << " account" << account;
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
    while (i.hasNext()) {
        i.next();
        if (!firstElement && !str.isEmpty()) {
            str += QLatin1Char('\n');
        }
        str += i.key() + QLatin1Char('\n');
        const TrayInfo trayInfo = i.value();
        if (trayInfo.hasAlert) {
            str += i18n("Has Alert") + QLatin1Char('\n');
            hasAlert = trayInfo.hasAlert;
        }
        if (trayInfo.unreadMessage != 0) {
            str += i18n("Has %1 Unread Message", trayInfo.unreadMessage);
        }
    }
    qDebug() << " str " << str;
    setToolTipSubTitle(str);
    if (status() == KStatusNotifierItem::Passive && (!str.isEmpty())) {
        setStatus(KStatusNotifierItem::Active);
    } else if (status() == KStatusNotifierItem::Active && (str.isEmpty())) {
        setStatus(KStatusNotifierItem::Passive);
    }
}

//Add Unity support
