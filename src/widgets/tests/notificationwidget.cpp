/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "notificationwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KNotification>
#include <KNotificationReplyAction>
#include <QDebug>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

NotificationWidget::NotificationWidget(QWidget *parent)
    : QWidget(parent)
    , mLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(mLineEdit);

    auto sendNotification = new QPushButton(u"Send"_s, this);
    mainLayout->addWidget(sendNotification);

    connect(sendNotification, &QPushButton::clicked, this, &NotificationWidget::slotSendNotification);
}

NotificationWidget::~NotificationWidget() = default;

void NotificationWidget::slotSendNotification()
{
    const QString str = mLineEdit->text();
    if (!str.isEmpty()) {
        auto notification = new KNotification(u"new-notification"_s, KNotification::CloseOnTimeout);
        notification->setTitle(u"test"_s);
        notification->setText(str);
        auto action = notification->addDefaultAction(u"Open Channel"_s);
        connect(action, &KNotificationAction::activated, this, []() {
            qDebug() << " default Activated !!!!!!";
        });
        connect(notification, &KNotification::closed, this, []() {
            qDebug() << " CLOSED!!!!!!!!!!!!!!!!!!!!!!!!!";
        });

        std::unique_ptr<KNotificationReplyAction> replyAction(new KNotificationReplyAction(u"Reply"_s));
        replyAction->setPlaceholderText(u"Reply…"_s);
        QObject::connect(replyAction.get(), &KNotificationReplyAction::replied, this, [](const QString &text) {
            qDebug() << " reply " << text;
        });
        notification->setReplyAction(std::move(replyAction));
        notification->sendEvent();
    }
}

#include "moc_notificationwidget.cpp"
