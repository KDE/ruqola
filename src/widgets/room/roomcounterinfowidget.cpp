/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

RoomCounterInfoWidget::~RoomCounterInfoWidget() = default;

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
        setText(i18np("%4 %1 new message since %2. %3",
                      "%4 %1 new messages since %2. %3",
                      mChannelCounterInfo.unreadMessages(),
                      mChannelCounterInfo.unreadFrom().toString(),
                      QStringLiteral(" <a href=\"markAsRead\">%1</a>").arg(i18n("(Mark As Read)")),
                      QStringLiteral("<a href=\"gotofirstunreadmessage\">%1</a>").arg(i18n("(Jump to first Unread)"))));
        animatedShow();
    } else {
        setVisible(false);
    }
}

void RoomCounterInfoWidget::slotLinkActivated(const QString &contents)
{
    if (contents == QLatin1String("markAsRead")) {
        Q_EMIT markAsRead();
    } else if (contents == QLatin1String("gotofirstunreadmessage")) {
        Q_EMIT jumpToUnreadMessage(mChannelCounterInfo.unreadMessages());
        setVisible(false);
    }
}

#include "moc_roomcounterinfowidget.cpp"
