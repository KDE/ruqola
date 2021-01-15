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

#include "usersinroomwidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "usersinroommenu.h"
#include "model/usersforroomfilterproxymodel.h"
#include "model/usersforroommodel.h"
#include <KLocalizedString>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>

UsersInRoomWidget::UsersInRoomWidget(QWidget *parent)
    : QWidget(parent)
    , mListView(new QListView(this))
    , mSearchLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setPlaceholderText(i18n("Search Channel..."));
    mSearchLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchLineEdit, this);
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &UsersInRoomWidget::slotTextChanged);
    mainLayout->addWidget(mSearchLineEdit);

    mListView->setObjectName(QStringLiteral("mListView"));
    mainLayout->addWidget(mListView);
    mListView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mListView, &QListView::customContextMenuRequested, this, &UsersInRoomWidget::slotCustomContextMenuRequested);
}

UsersInRoomWidget::~UsersInRoomWidget()
{

}

void UsersInRoomWidget::slotTextChanged(const QString &str)
{
    //TODO
}

void UsersInRoomWidget::setRoom(Room *room)
{
    mRoom = room;
    if (mRoom) {
        const auto model = Ruqola::self()->rocketChatAccount()->usersForRoomFilterProxyModel(mRoom->roomId());
        mListView->setModel(model);
//        connect(model, &UsersForRoomFilterProxyModel::rowsInserted, this, &UsersInRoomFlowWidget::generateListUsersWidget);
//        connect(model, &UsersForRoomFilterProxyModel::rowsRemoved, this, &UsersInRoomFlowWidget::generateListUsersWidget);
//        connect(model, &UsersForRoomFilterProxyModel::dataChanged, this, &UsersInRoomFlowWidget::updateListUsersWidget);
//        connect(model, &UsersForRoomFilterProxyModel::modelReset, this, &UsersInRoomFlowWidget::generateListUsersWidget);
//        connect(model, &UsersForRoomFilterProxyModel::hasFullListChanged, this, &UsersInRoomFlowWidget::generateListUsersWidget);
//        generateListUsersWidget();
    }
}

void UsersInRoomWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    if (!mMenu) {
        mMenu = new UsersInRoomMenu(this);
    }
    QModelIndex index = mListView->indexAt(pos);
    if (index.isValid()) {
        const QString userId = index.data(UsersForRoomModel::UsersForRoomRoles::UserId).toString();
        const QString userName = index.data(UsersForRoomModel::UsersForRoomRoles::UserName).toString();
        mMenu->setUserId(userId);
        mMenu->setUserName(userName);
    }
    mMenu->setParentWidget(this);
    mMenu->setRoom(mRoom);
    mMenu->slotCustomContextMenuRequested(pos);
}
