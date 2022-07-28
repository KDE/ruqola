/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "room.h"
#include <QPointer>
#include <QStackedWidget>
#include <QWidget>
class ChannelListWidget;
class RoomWidget;
class QSplitter;
class RocketChatAccount;
class Room;
class BannerMessageWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RuqolaMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuqolaMainWidget(QWidget *parent = nullptr);
    ~RuqolaMainWidget() override;
    Q_REQUIRED_RESULT QString roomId() const;
    void setCurrentRocketChatAccount(RocketChatAccount *account);
    Q_REQUIRED_RESULT Room::RoomType roomType() const;
    Room *room() const;

    void selectChannelRoom(const QString &roomName, const QString &roomId, Room::RoomType roomType);
    void selectNextUnreadChannel();
Q_SIGNALS:
    void channelSelected();

private:
    void showEvent(QShowEvent *event) override;
    void updateBannerInfo();

    QSplitter *const mSplitter;
    ChannelListWidget *const mChannelList;
    QStackedWidget *const mStackedRoomWidget;
    RoomWidget *const mRoomWidget;
    QWidget *const mEmptyRoomWidget;
    BannerMessageWidget *const mBannerMessageWidget;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};
