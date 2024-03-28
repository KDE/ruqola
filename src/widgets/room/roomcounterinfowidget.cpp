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
    setPosition(KMessageWidget::Header);
    connect(this, &KMessageWidget::linkActivated, this, &RoomCounterInfoWidget::slotLinkActivated);
}

RoomCounterInfoWidget::~RoomCounterInfoWidget() = default;

const ChannelCounterInfo *RoomCounterInfoWidget::channelCounterInfo() const
{
    return mChannelCounterInfo;
}

void RoomCounterInfoWidget::setChannelCounterInfo(const ChannelCounterInfo *channelCounterInfo)
{
    if (!mChannelCounterInfo) {
        mChannelCounterInfo = channelCounterInfo;
        updateInfo();
    } else {
        if (channelCounterInfo) {
            if (*mChannelCounterInfo != *channelCounterInfo) {
                mChannelCounterInfo = channelCounterInfo;
                updateInfo();
            }
        } else {
            mChannelCounterInfo = nullptr;
            updateInfo();
        }
    }
}

void RoomCounterInfoWidget::updateInfo()
{
    if (mChannelCounterInfo && mChannelCounterInfo->isValid() && mChannelCounterInfo->unreadMessages() > 0) {
        setText(i18np("%4 %1 new message since %2. %3",
                      "%4 %1 new messages since %2. %3",
                      mChannelCounterInfo->unreadMessages(),
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
    if (contents == QLatin1StringView("markAsRead")) {
        Q_EMIT markAsRead();
    } else if (contents == QLatin1StringView("gotofirstunreadmessage")) {
        Q_EMIT jumpToUnreadMessage(mChannelCounterInfo->unreadMessages());
        setVisible(false);
    }
}

#include "moc_roomcounterinfowidget.cpp"
