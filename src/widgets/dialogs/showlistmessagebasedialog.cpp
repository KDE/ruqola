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

#include "showlistmessagebasedialog.h"
#include "rocketchataccount.h"
#include "room/messagelistview.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "showlistmessagebasewidget.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>

ShowListMessageBaseDialog::ShowListMessageBaseDialog(QWidget *parent)
    : QDialog(parent)
    , mShowListMessage(new ShowListMessageBaseWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mShowListMessage->setObjectName(QStringLiteral("mShowListMessage"));
    mainLayout->addWidget(mShowListMessage);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ShowListMessageBaseDialog::reject);
    connect(mShowListMessage, &ShowListMessageBaseWidget::loadMoreElements, this, &ShowListMessageBaseDialog::slotLoadMoreMessages);

    connect(mShowListMessage->messageListView(), &MessageListView::goToMessageRequested, this, &ShowListMessageBaseDialog::goToMessageRequested);
}

ShowListMessageBaseDialog::~ShowListMessageBaseDialog()
{
}

void ShowListMessageBaseDialog::slotLoadMoreMessages()
{
    if (mRoomId.isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "RoomId is empty. It's a bug";
        return;
    }
    Ruqola::self()->rocketChatAccount()->loadMoreListMessages(roomId());
}

void ShowListMessageBaseDialog::setModel(ListMessagesModelFilterProxyModel *model)
{
    mShowListMessage->setModel(model);
}

void ShowListMessageBaseDialog::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString ShowListMessageBaseDialog::roomId() const
{
    return mRoomId;
}

void ShowListMessageBaseDialog::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    mShowListMessage->setCurrentRocketChatAccount(currentRocketChatAccount);
}

void ShowListMessageBaseDialog::setRoom(Room *room)
{
    mShowListMessage->setRoom(room);
}
