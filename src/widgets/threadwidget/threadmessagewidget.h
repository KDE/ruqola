/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class RocketChatAccount;
class Room;
class QLabel;
class RoomWidgetBase;
class QToolButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ThreadMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ThreadMessageWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ThreadMessageWidget() override;

    Q_REQUIRED_RESULT QString threadMessageId() const;
    void setThreadMessageId(const QString &threadMessageId);


    void setThreadPreview(const QString &preview);

    void setRoom(Room *room);

    void setFollowingThread(bool threadIsFollowing);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void intialize();
    void slotCreateNewDiscussion(const QString &messageId, const QString &originalMessage);
    void updateFollowThreadIcon(bool followThread);
    void slotFollowThreadChanged(bool clicked);
    QString mThreadMessageId;
    QLabel *const mThreadPreview;
    RoomWidgetBase *const mRoomWidgetBase;
    RocketChatAccount *const mRocketChatAccount;
    QToolButton *const mFollowButton;
};

