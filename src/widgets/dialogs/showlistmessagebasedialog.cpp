/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showlistmessagebasedialog.h"
using namespace Qt::Literals::StringLiterals;

#include "rocketchataccount.h"
#include "room/messagelistview.h"
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
    mainLayout->setObjectName(u"mainLayout"_s);

    mShowListMessage->setObjectName(u"mShowListMessage"_s);
    mainLayout->addWidget(mShowListMessage);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
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

void ShowListMessageBaseDialog::setFilterProxyModel(ListMessagesFilterProxyModel *model)
{
    mShowListMessage->setFilterProxyModel(model);
}

void ShowListMessageBaseDialog::setFilteringByType(ListMessagesFilterProxyModel::FilteringByType type)
{
    mShowListMessage->setFilteringByType(type);
}

void ShowListMessageBaseDialog::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

QByteArray ShowListMessageBaseDialog::roomId() const
{
    return mRoomId;
}

void ShowListMessageBaseDialog::setRoom(Room *room)
{
    mShowListMessage->setRoom(room);
}

void ShowListMessageBaseDialog::addMessageLineWidget(QWidget *w)
{
    mShowListMessage->addMessageLineWidget(w);
}

#include "moc_showlistmessagebasedialog.cpp"
