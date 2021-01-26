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
#include "roomcounterinfowidget.h"
#include <KLocalizedString>
RoomCounterInfoWidget::RoomCounterInfoWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Information);
    connect(this, &KMessageWidget::linkActivated, this, &RoomCounterInfoWidget::slotLinkActivated);
}

RoomCounterInfoWidget::~RoomCounterInfoWidget()
{
}

ChannelCounterInfo RoomCounterInfoWidget::channelCounterInfo() const
{
    return mChannelCounterInfo;
}

void RoomCounterInfoWidget::setChannelCounterInfo(const ChannelCounterInfo &channelCounterInfo)
{
    if (mChannelCounterInfo != channelCounterInfo) {
        mChannelCounterInfo = channelCounterInfo;
        updateInfo();
    }
}

void RoomCounterInfoWidget::updateInfo()
{
    if (mChannelCounterInfo.isValid() && mChannelCounterInfo.unreadMessages() > 0) {
        setText(i18np("%1 new message since %2. %3",
                      "%1 new messages since %2. %3",
                      mChannelCounterInfo.unreadMessages(),
                      mChannelCounterInfo.unreadFrom().toString(),
                      QStringLiteral(" <a href=\"markAsRead\">%1</a>").arg(i18n("(Mark As Read)"))));
        animatedShow();
    } else {
        setVisible(false);
    }
}

void RoomCounterInfoWidget::slotLinkActivated(const QString &contents)
{
    if (contents == QLatin1String("markAsRead")) {
        Q_EMIT markAsRead();
    }
}
