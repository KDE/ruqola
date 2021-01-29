/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "roomquotemessagewidget.h"
#include <KLocalizedString>
#include <KStandardGuiItem>
#include <QAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

RoomQuoteMessageWidget::RoomQuoteMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setCloseButtonVisible(false);
    setMessageType(Information);
    setWordWrap(true);
    setVisible(false);
    auto cancelReplyingInThreadAction = new QAction(i18n("Cancel"), this);
    connect(cancelReplyingInThreadAction, &QAction::triggered, this, &RoomQuoteMessageWidget::cancelQuoteMessage);
    addAction(cancelReplyingInThreadAction);
}

RoomQuoteMessageWidget::~RoomQuoteMessageWidget()
{
}

void RoomQuoteMessageWidget::setMessageText(const QString &str)
{
    setText(i18n("Quote Message: \'%1\'", str));
}
