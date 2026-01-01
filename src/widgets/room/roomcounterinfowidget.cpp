/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "roomcounterinfowidget.h"
#include <KLocalizedString>
using namespace Qt::Literals::StringLiterals;
RoomCounterInfoWidget::RoomCounterInfoWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Information);
    setPosition(KMessageWidget::Header);
    setTextFormat(Qt::PlainText);

    auto jumpToFirstUnreadAction = new QAction(QIcon::fromTheme(u"go-jump-locationbar"_s), i18nc("@action:button", "Jump to First Unread"), this);
    connect(jumpToFirstUnreadAction, &QAction::triggered, this, [this] {
        Q_EMIT jumpToUnreadMessage(mChannelCounterInfo->unreadMessages());
        setVisible(false);
    });
    addAction(jumpToFirstUnreadAction);

    auto markAsReadAction = new QAction(QIcon::fromTheme(u"checkmark-symbolic"_s), i18nc("@action:button", "Mark as Read"), this);
    connect(markAsReadAction, &QAction::triggered, this, [this] {
        Q_EMIT markAsRead();
    });
    addAction(markAsReadAction);
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
        setText(i18np("%1 new message since %2.",
                      "%1 new messages since %2.",
                      mChannelCounterInfo->unreadMessages(),
                      QLocale().toString(mChannelCounterInfo->unreadFrom().toLocalTime(), QLocale::ShortFormat)));
        setVisible(true); // FIXME: AnimateShow create some pb. Need to investigate it
    } else {
        setVisible(false);
    }
}

#include "moc_roomcounterinfowidget.cpp"
