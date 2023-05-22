/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "notificationwidget.h"
#include <KNotification>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

NotificationWidget::NotificationWidget(QWidget *parent)
    : QWidget(parent)
    , mLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(mLineEdit);

    auto sendNotification = new QPushButton(QStringLiteral("Send"), this);
    mainLayout->addWidget(sendNotification);

    connect(sendNotification, &QPushButton::clicked, this, &NotificationWidget::slotSendNotification);
}

NotificationWidget::~NotificationWidget() = default;

void NotificationWidget::slotSendNotification()
{
    const QString str = mLineEdit->text();
    if (!str.isEmpty()) {
        KNotification::event(KNotification::Notification, QStringLiteral("title:") + str, QStringLiteral("message:") + str.toHtmlEscaped(), QPixmap());
    }
}
