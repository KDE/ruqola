/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channellist/channellistwidget.h"
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

    void selectNextUnreadChannel();

Q_SIGNALS:
    void channelSelected();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void selectChannelRoom(const ChannelListView::ChannelSelectedInfo &roomInfo);
    LIBRUQOLAWIDGETS_NO_EXPORT void showEvent(QShowEvent *event) override;
    LIBRUQOLAWIDGETS_NO_EXPORT void updateBannerInfo();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotBannerDismissDone();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotMarkBannerAsRead(const QString &identifier);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRoomPressed(const QString &roomId);
    LIBRUQOLAWIDGETS_NO_EXPORT void createBannerMessageWidget();

    QSplitter *const mSplitter;
    ChannelListWidget *const mChannelList;
    QStackedWidget *const mStackedRoomWidget;
    RoomWidget *const mRoomWidget;
    QWidget *const mEmptyRoomWidget;
    BannerMessageWidget *mBannerMessageWidget = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    QVBoxLayout *const mTopLayout;
};
