/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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
#include "knotifications_version.h"
#include "ruqola_debug.h"
#include <KLocalizedString>
#include <KNotification>
#include <KNotificationReplyAction>
#define ADD_REPLY_NOTIFICATION 1
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
        auto notification = new KNotification(QStringLiteral("new-notification"), KNotification::CloseOnTimeout);
        notification->setTitle(mInfo.title);
        const QString userName = mInfo.senderName.isEmpty() ? mInfo.senderUserName : mInfo.senderName;
        notification->setText(i18n("%1: %2", userName, mInfo.message.toHtmlEscaped()));
        if (!mInfo.pixmap.isNull()) {
            notification->setPixmap(mInfo.pixmap);
        }
        notification->setDefaultAction(i18nc("Open channel", "Open Channel"));
        connect(notification, &KNotification::defaultActivated, this, &NotifierJob::slotDefaultActionActivated);
        connect(notification, &KNotification::closed, this, &NotifierJob::deleteLater);

#ifdef ADD_REPLY_NOTIFICATION
        std::unique_ptr<KNotificationReplyAction> replyAction(new KNotificationReplyAction(i18n("Reply")));
        replyAction->setPlaceholderText(i18n("Reply..."));
        QObject::connect(replyAction.get(), &KNotificationReplyAction::replied, this, [this](const QString &text) {
            // qDebug() << " mInfo " << mInfo;
            Q_EMIT sendReply(text, mInfo.roomId);
            // qDebug() << " reply " << text;
        });
        notification->setReplyAction(std::move(replyAction));
#endif
        notification->sendEvent();
    } else {
        qCWarning(RUQOLA_LOG) << "Info is invalid";
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
