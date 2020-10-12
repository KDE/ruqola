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

#include "usersinroomflowwidget.h"
#include "usersinroomlabel.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "common/flowlayout.h"
#include "room.h"
#include "model/usersforroommodel.h"
#include "model/usersforroomfilterproxymodel.h"
#include <KLocalizedString>
#include <QLabel>

UsersInRoomFlowWidget::UsersInRoomFlowWidget(QWidget *parent)
    : QWidget(parent)
{
    mFlowLayout = new FlowLayout(this);
    mFlowLayout->setObjectName(QStringLiteral("mFlowLayout"));
    mFlowLayout->setSpacing(0);
    mFlowLayout->setContentsMargins({});
}

UsersInRoomFlowWidget::~UsersInRoomFlowWidget()
{
}

void UsersInRoomFlowWidget::setRoom(Room *room)
{
    mRoom = room;
    if (mRoom) {
        const auto model = Ruqola::self()->rocketChatAccount()->usersForRoomFilterProxyModel(mRoom->roomId());
        connect(model, &UsersForRoomFilterProxyModel::rowsInserted, this, &UsersInRoomFlowWidget::generateListUsersWidget);
        connect(model, &UsersForRoomFilterProxyModel::rowsRemoved, this, &UsersInRoomFlowWidget::generateListUsersWidget);
        connect(model, &UsersForRoomFilterProxyModel::dataChanged, this, &UsersInRoomFlowWidget::updateListUsersWidget);
        connect(model, &UsersForRoomFilterProxyModel::modelReset, this, &UsersInRoomFlowWidget::generateListUsersWidget);
        connect(model, &UsersForRoomFilterProxyModel::hasFullListChanged, this, &UsersInRoomFlowWidget::generateListUsersWidget);
        generateListUsersWidget();
    } else {
        mFlowLayout->clearAndDeleteWidgets();
    }
}

void UsersInRoomFlowWidget::showEvent(QShowEvent *event)
{
    generateListUsersWidget();
    QWidget::showEvent(event);
}

void UsersInRoomFlowWidget::updateListUsersWidget(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if (isVisible()) {
        for (int row = topLeft.row(), total = bottomRight.row(); row <= total; ++row) {
            const QModelIndex userModelIndex = topLeft.sibling(row, 0);
            const QString userId = userModelIndex.data(UsersForRoomModel::UsersForRoomRoles::UserId).toString();

            UsersInRoomLabel *userLabel = mListUsersWidget.value(userId);
            if (userLabel) {
                const QString userDisplayName = userModelIndex.data(UsersForRoomModel::UsersForRoomRoles::DisplayName).toString();
                const QString iconStatus = userModelIndex.data(UsersForRoomModel::UsersForRoomRoles::IconStatus).toString();
                const QString userName = userModelIndex.data(UsersForRoomModel::UsersForRoomRoles::UserName).toString();
                //qDebug() << " updating userdId " << userId << " userName " << userName << "  info.iconStatus " << iconStatus;
                UsersInRoomLabel::UserInfo info;
                info.userDisplayName = userDisplayName;
                info.iconStatus = iconStatus;
                info.userId = userId;
                info.userName = userName;
                userLabel->setUserInfo(info);
            }
        }
    }
}

void UsersInRoomFlowWidget::generateListUsersWidget()
{
    if (isVisible()) {
        const auto model = Ruqola::self()->rocketChatAccount()->usersForRoomFilterProxyModel(mRoom->roomId());
        const auto count = model->rowCount();
        mFlowLayout->clearAndDeleteWidgets();
        mListUsersWidget.clear();
        for (int i = 0; i < count; ++i) {
            const auto userModelIndex = model->index(i, 0);
            const QString userDisplayName = userModelIndex.data(UsersForRoomModel::UsersForRoomRoles::DisplayName).toString();
            const QString iconStatus = userModelIndex.data(UsersForRoomModel::UsersForRoomRoles::IconStatus).toString();
            const QString userId = userModelIndex.data(UsersForRoomModel::UsersForRoomRoles::UserId).toString();
            const QString userName = userModelIndex.data(UsersForRoomModel::UsersForRoomRoles::UserName).toString();
            UsersInRoomLabel::UserInfo info;
            info.userDisplayName = userDisplayName;
            info.iconStatus = iconStatus;
            info.userId = userId;
            info.userName = userName;
            UsersInRoomLabel *userLabel = new UsersInRoomLabel(this);
            userLabel->setUserInfo(info);
            userLabel->setRoom(mRoom);
            mFlowLayout->addWidget(userLabel);
            mListUsersWidget.insert(userId, userLabel);
        }
        if (count > 0) {
            if (!model->hasFullList()) {
                QLabel *loadingMoreLabel = new QLabel(QStringLiteral("<a href=\"loadmoreelement\">%1</a>").arg(i18n("(Click here for Loading more...)")), this);
                loadingMoreLabel->setTextFormat(Qt::RichText);
                loadingMoreLabel->setContextMenuPolicy(Qt::CustomContextMenu);
                connect(loadingMoreLabel, &QLabel::linkActivated, this, &UsersInRoomFlowWidget::loadMoreUsersAttachment);
                mFlowLayout->addWidget(loadingMoreLabel);
            }
        }
    }
}

void UsersInRoomFlowWidget::loadMoreUsersAttachment()
{
    Ruqola::self()->rocketChatAccount()->loadMoreUsersInRoom(mRoom->roomId(), mRoom->channelType());
}
