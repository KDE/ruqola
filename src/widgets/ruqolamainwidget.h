/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#ifndef RUQOLAMAINWIDGET_H
#define RUQOLAMAINWIDGET_H

#include <QStackedWidget>
#include <QWidget>
#include "libruqolawidgets_private_export.h"
class ChannelListWidget;
class RoomWidget;
class QSplitter;
class RocketChatAccount;
class RoomWrapper;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RuqolaMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuqolaMainWidget(QWidget *parent = nullptr);
    ~RuqolaMainWidget();
    Q_REQUIRED_RESULT QString roomId() const;
    void setCurrentRocketChatAccount(RocketChatAccount *account);
    Q_REQUIRED_RESULT QString roomType() const;
    RoomWrapper *roomWrapper() const;

Q_SIGNALS:
    void channelSelected();
private:
    void slotSelectChannelRequested(const QString &channelId);
    void selectChannelRoom(const QModelIndex &index);
    ChannelListWidget *mChannelList = nullptr;
    RoomWidget *mRoomWidget = nullptr;
    QSplitter *mSplitter = nullptr;
    RocketChatAccount *mCurrentRocketChatAccount = nullptr;
    QStackedWidget *mStackedRoomWidget = nullptr;
    QWidget *mEmptyRoomWidget = nullptr;
};

#endif // RUQOLAMAINWIDGET_H
