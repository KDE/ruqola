/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

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
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    setPosition(KMessageWidget::Header);
#endif
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
                      QLocale().toString(mChannelCounterInfo->unreadFrom()),
                      QStringLiteral(" <a href=\"markAsRead\">%1</a>").arg(i18n("(Mark As Read)")),
                      QStringLiteral("<a href=\"gotofirstunreadmessage\">%1</a>").arg(i18n("(Jump to first Unread)"))));
        setVisible(true); // FIXME: AnimateShow create some pb. Need to investigate it
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
