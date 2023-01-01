/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    QString strToDisplay = str;
    if (strToDisplay.length() > 80) {
        strToDisplay = strToDisplay.left(80) + QStringLiteral("...");
    }
    setText(i18n("Reply in Thread for Message: \'%1\'", strToDisplay));
}
