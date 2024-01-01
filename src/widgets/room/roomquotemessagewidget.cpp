/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomquotemessagewidget.h"
#include <KLocalizedString>
#include <QAction>
#include <QPushButton>

RoomQuoteMessageWidget::RoomQuoteMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setCloseButtonVisible(false);
    setMessageType(Information);
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    setPosition(KMessageWidget::Header);
#endif
    setWordWrap(true);
    setVisible(false);
    auto cancelReplyingInThreadAction = new QAction(i18n("Cancel"), this);
    connect(cancelReplyingInThreadAction, &QAction::triggered, this, &RoomQuoteMessageWidget::cancelQuoteMessage);
    addAction(cancelReplyingInThreadAction);
}

RoomQuoteMessageWidget::~RoomQuoteMessageWidget() = default;

void RoomQuoteMessageWidget::setMessageText(const QString &str)
{
    setText(i18n("Quote Message: \'%1\'", str));
}

#include "moc_roomquotemessagewidget.cpp"
