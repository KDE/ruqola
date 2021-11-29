/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class RocketChatAccount;
class Room;
class QLabel;
class RoomWidgetBase;
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

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void intialize();
    void slotCreateNewDiscussion(const QString &messageId, const QString &originalMessage);
    QString mThreadMessageId;
    QLabel *const mThreadPreview;
    RoomWidgetBase *const mRoomWidgetBase;
    RocketChatAccount *const mRocketChatAccount;
};

