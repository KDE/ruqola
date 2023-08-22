/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notifierjob.h"
#include "ruqola_debug.h"
#include <KLocalizedString>
#include <KNotification>
#include <KNotificationReplyAction>
#include <KWindowSystem>

NotifierJob::NotifierJob(QObject *parent)
    : QObject(parent)
{
}

NotifierJob::~NotifierJob() = default;

void NotifierJob::start()
{
    if (mInfo.isValid()) {
        auto notification = new KNotification(QStringLiteral("new-notification"), KNotification::CloseOnTimeout);
        notification->setTitle(mInfo.title());
        const QString userName = mInfo.senderName().isEmpty() ? mInfo.senderUserName() : mInfo.senderName();
        notification->setText(i18n("%1: %2", userName, mInfo.message().toHtmlEscaped()));
        if (!mInfo.pixmap().isNull()) {
            notification->setPixmap(mInfo.pixmap());
        }

        auto openChannel = [this, notification] {
            KWindowSystem::setCurrentXdgActivationToken(notification->xdgActivationToken());
            Q_EMIT switchToAccountAndRoomName(mInfo.accountName(), mInfo.roomId(), mInfo.channelType());
        };

#if QT_VERSION_MAJOR == 6
        auto defaultAction = notification->addDefaultAction(i18nc("Open channel", "Open Channel"));
        connect(defaultAction, &KNotificationAction::activated, this, openChannel);
#else
        notification->setDefaultAction(i18nc("Open channel", "Open Channel"));
        connect(notification, &KNotification::defaultActivated, this, openChannel);
#endif
        connect(notification, &KNotification::closed, this, &NotifierJob::deleteLater);

        std::unique_ptr<KNotificationReplyAction> replyAction(new KNotificationReplyAction(i18n("Reply")));
        replyAction->setPlaceholderText(i18n("Reply..."));
        QObject::connect(replyAction.get(), &KNotificationReplyAction::replied, this, [this](const QString &text) {
            // qDebug() << " mInfo " << mInfo;
            Q_EMIT sendReply(text, mInfo.roomId(), mInfo.tmId());
            // qDebug() << " reply " << text;
        });
        notification->setReplyAction(std::move(replyAction));
        notification->sendEvent();
    } else {
        qCWarning(RUQOLA_LOG) << "Info is invalid";
        deleteLater();
    }
}

NotificationInfo NotifierJob::info() const
{
    return mInfo;
}

void NotifierJob::setInfo(const NotificationInfo &info)
{
    mInfo = info;
}

#include "moc_notifierjob.cpp"
