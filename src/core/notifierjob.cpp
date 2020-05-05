/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "notifierjob.h"
#include "ruqola_debug.h"

#include <KNotification>

NotifierJob::NotifierJob(QObject *parent)
    : QObject(parent)
{
}

NotifierJob::~NotifierJob()
{
}

void NotifierJob::start()
{
    if (mInfo.isValid()) {
        KNotification *notification = new KNotification(QStringLiteral("new-notification"), nullptr, KNotification::CloseOnTimeout);
        notification->setTitle(mInfo.title);
        notification->setText(mInfo.message.toHtmlEscaped());
        if (!mInfo.pixmap.isNull()) {
            notification->setPixmap(mInfo.pixmap);
        }
        connect(notification, &KNotification::defaultActivated, this, &NotifierJob::slotDefaultActionActivated);
        connect(notification, &KNotification::closed, this, &NotifierJob::deleteLater);

        notification->sendEvent();
    } else {
        qCWarning(RUQOLA_LOG) << "Info is Empty";
        deleteLater();
    }
}

Utils::NotificationInfo NotifierJob::info() const
{
    return mInfo;
}

void NotifierJob::setInfo(const Utils::NotificationInfo &info)
{
    mInfo = info;
}

void NotifierJob::slotDefaultActionActivated()
{
    qDebug() << " void NotifierJob::slotDefaultActionActivated()";
    Q_EMIT switchToAccountAndRoomName(mAccountName, mInfo.roomName, mInfo.channelType);
}

QString NotifierJob::accountName() const
{
    return mAccountName;
}

void NotifierJob::setAccountName(const QString &accountName)
{
    mAccountName = accountName;
}
