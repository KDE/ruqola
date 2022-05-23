/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showlistmessagebasedialog.h"
#include "rocketchataccount.h"
#include "room/messagelistview.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "showlistmessagebasewidget.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>

ShowListMessageBaseDialog::ShowListMessageBaseDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mShowListMessage(new ShowListMessageBaseWidget(account, this))
    , mRocketChatAccount(account)
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

ShowListMessageBaseDialog::~ShowListMessageBaseDialog() = default;

void ShowListMessageBaseDialog::slotLoadMoreMessages()
{
    if (mRoomId.isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "RoomId is empty. It's a bug";
        return;
    }
    mRocketChatAccount->loadMoreListMessages(roomId());
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

void ShowListMessageBaseDialog::setRoom(Room *room)
{
    mShowListMessage->setRoom(room);
}
