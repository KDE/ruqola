/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "libruqolawidgets_private_export.h"
#include <QPointer>
#include <QWidget>
class ChannelListView;
class QLineEdit;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelListWidget(QWidget *parent = nullptr);
    ~ChannelListWidget() override;

    ChannelListView *channelListView() const;

    void setCurrentRocketChatAccount(RocketChatAccount *account);

Q_SIGNALS:
    void roomSelected(const QString &roomId, const QString &roomType);

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    void slotAccountInitialized();
    void slotSearchRoomTextChanged();
    void clearFilterChannel();
    void slotOpenLinkRequested(const QString &link);

    ChannelListView *mChannelView = nullptr;
    QLineEdit *mSearchRoom = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};

