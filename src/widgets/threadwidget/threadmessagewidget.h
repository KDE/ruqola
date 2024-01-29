/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "messages/message.h"
#include <QPointer>
#include <QWidget>
class RocketChatAccount;
class Room;
class QLabel;
class RoomWidgetBase;
class QToolButton;

namespace TextEditTextToSpeech
{
class TextToSpeechContainerWidget;
}

class LIBRUQOLAWIDGETS_TESTS_EXPORT ThreadMessageWidget : public QWidget
{
    Q_OBJECT
public:
    struct LIBRUQOLAWIDGETS_TESTS_EXPORT ThreadMessageInfo {
        Message messageThread;
        QString threadMessageId;
        QString threadMessagePreview;
        Room *room = nullptr;
        bool threadIsFollowing = false;
    };
    explicit ThreadMessageWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ThreadMessageWidget() override;

    void setThreadMessageInfo(const ThreadMessageWidget::ThreadMessageInfo &info);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void initialize();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCreateNewDiscussion(const QString &messageId, const QString &originalMessage);
    LIBRUQOLAWIDGETS_NO_EXPORT void updateFollowThreadIcon(bool followThread);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFollowThreadChanged(bool clicked);
    QString mThreadMessageId;
    QLabel *const mThreadPreview;
    RoomWidgetBase *const mRoomWidgetBase;
    RocketChatAccount *const mRocketChatAccount;
    QToolButton *const mFollowButton;
    QPointer<Room> mRoom;
    TextEditTextToSpeech::TextToSpeechContainerWidget *const mTextToSpeechWidget = nullptr;
};
