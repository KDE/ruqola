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

#pragma once

#include <QWidget>

#include "channelsearchnamelineedit.h"
#include "libruqolawidgets_private_export.h"
class QStackedWidget;
class ChannelSearchNameLineEdit;
class ChannelSearchNameLineResultWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelSearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelSearchWidget(QWidget *parent = nullptr);
    ~ChannelSearchWidget() override;

    Q_REQUIRED_RESULT QString channelName() const;

    void setChannelInfo(const QString &name, const QString &channelId);

    Q_REQUIRED_RESULT QString channelId() const;
Q_SIGNALS:
    void updateRoomName(bool valid);

private:
    void slotClearRoom();
    void slotSelectedRoom(const ChannelSearchNameLineEdit::ChannelCompletionInfo &userInfo);
    ChannelSearchNameLineEdit::ChannelCompletionInfo mUserInfo;
    QStackedWidget *const mStackedWidget;
    ChannelSearchNameLineEdit *const mChannelSearchNameLineEdit;
    ChannelSearchNameLineResultWidget *const mChannelSearchLineResult;
};
