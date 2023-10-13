/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationwidget.h"
#include <QApplication>
#include <QCommandLineParser>
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);
    auto w = new NotificationWidget();

#if 0
    auto notification = new KNotification(QStringLiteral("new-notification"), KNotification::CloseOnTimeout);
    notification->setTitle(QStringLiteral("test"));
    const QString userName = mInfo.senderName().isEmpty() ? mInfo.senderUserName() : mInfo.senderName();
    notification->setText(i18n("%1: %2", userName, mInfo.message().toHtmlEscaped()));
    if (!mInfo.pixmap().isNull()) {
        notification->setPixmap(mInfo.pixmap());
    }
    notification->setDefaultAction(i18nc("Open channel", "Open Channel"));
    connect(notification, &KNotification::defaultActivated, this, &NotifierJob::slotDefaultActionActivated);
    connect(notification, &KNotification::closed, this, [this]() {
        qDebug() << " CLOSED!!!!!!!!!!!!!!!!!!!!!!!!!";
        deleteLater();
    });
    connect(notification, &KNotification::ignored, this, [this] () {
        qDebug() << " IGNORED !!!!!!!!!!!!!!!!!!!!!!!!!!!";
        deleteLater();
    });

    std::unique_ptr<KNotificationReplyAction> replyAction(new KNotificationReplyAction(i18n("Reply")));
    replyAction->setPlaceholderText(i18n("Reply..."));
    QObject::connect(replyAction.get(), &KNotificationReplyAction::replied, this, [this](const QString &text) {
        // qDebug() << " mInfo " << mInfo;
        Q_EMIT sendReply(text, mInfo.roomId(), mInfo.tmId());
        // qDebug() << " reply " << text;
    });
    notification->setReplyAction(std::move(replyAction));
    notification->sendEvent();

#endif
    w->resize(800, 600);
    w->show();
    return app.exec();
}
