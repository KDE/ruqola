/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef ROOMWIDGETBASE_H
#define ROOMWIDGETBASE_H

#include "libruqolawidgets_private_export.h"
#include "messagelistview.h"
#include <QWidget>

class UploadFileProgressStatusWidget;
class RoomReplyThreadWidget;
class RoomQuoteMessageWidget;
class QStackedWidget;
class MessageLineWidget;
class ReadOnlyLineEditWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RoomWidgetBase : public QWidget
{
    Q_OBJECT
public:
    explicit RoomWidgetBase(MessageListView::Mode mode, QWidget *parent = nullptr);
    ~RoomWidgetBase() override;

    Q_REQUIRED_RESULT MessageListView *messageListView() const;

    Q_REQUIRED_RESULT MessageLineWidget *messageLineWidget() const;

Q_SIGNALS:
    void cleanNotification();

private:
    void slotShowQuoteMessage(const QString &permalink, const QString &text);
    void slotCreateNewDiscussion(const QString &messageId, const QString &originalMessage);
    void slotCreatePrivateDiscussion(const QString &userName);
    void keyPressedInLineEdit(QKeyEvent *ev);
    UploadFileProgressStatusWidget *const mUploadFileProgressStatusWidget;
    MessageListView *const mMessageListView;
    RoomReplyThreadWidget *const mRoomReplyThreadWidget;
    RoomQuoteMessageWidget *const mRoomQuoteMessageWidget;
    QStackedWidget *const mStackedWidget;
    MessageLineWidget *const mMessageLineWidget;
    ReadOnlyLineEditWidget *const mReadOnlyLineEditWidget;
};

#endif // ROOMWIDGETBASE_H
