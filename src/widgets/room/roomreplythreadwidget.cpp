/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "roomreplythreadwidget.h"

#include <KLocalizedString>
#include <QAction>
#include <QPushButton>

RoomReplyThreadWidget::RoomReplyThreadWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setCloseButtonVisible(false);
    setMessageType(Information);
    setWordWrap(true);
    setText(i18n("Replying in a thread"));
    setVisible(false);
    auto cancelReplyingInThreadAction = new QAction(i18n("Cancel"), this);
    connect(cancelReplyingInThreadAction, &QAction::triggered, this, &RoomReplyThreadWidget::cancelReplyingInThread);
    addAction(cancelReplyingInThreadAction);
}

RoomReplyThreadWidget::~RoomReplyThreadWidget() = default;

void RoomReplyThreadWidget::setMessageText(const QString &str)
{
    setText(i18n("Reply in Thread for Message: \'%1\'", str));
}
