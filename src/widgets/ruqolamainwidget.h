/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "room.h"
#include <QPointer>
#include <QStackedWidget>
#include <QWidget>
class QVBoxLayout;
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
    [[nodiscard]] QString roomId() const;
    void setCurrentRocketChatAccount(RocketChatAccount *account);
    [[nodiscard]] Room::RoomType roomType() const;
    Room *room() const;

    void selectChannelRoom(const QString &roomName, const QString &roomId, Room::RoomType roomType);
    void selectNextUnreadChannel();

Q_SIGNALS:
    void channelSelected();

private:
    void showEvent(QShowEvent *event) override;
    void updateBannerInfo();
    void slotBannerDismissDone();
    void slotMarkBannerAsRead(const QString &identifier);
    void slotRoomPressed(const QString &roomId);
    void createBannerMessageWidget();

    QSplitter *const mSplitter;
    ChannelListWidget *const mChannelList;
    QStackedWidget *const mStackedRoomWidget;
    RoomWidget *const mRoomWidget;
    QWidget *const mEmptyRoomWidget;
    BannerMessageWidget *mBannerMessageWidget = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    QVBoxLayout *const mTopLayout;
};
