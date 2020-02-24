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
#include "roomwrapper.h"
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
    mFlowLayout->setContentsMargins(0, 0, 0, 0);
}

UsersInRoomFlowWidget::~UsersInRoomFlowWidget()
{
}

void UsersInRoomFlowWidget::setRoomWrapper(RoomWrapper *roomWrapper)
{
    mRoomWrapper = roomWrapper;
    if (mRoomWrapper) {
        const auto model = Ruqola::self()->rocketChatAccount()->usersForRoomFilterProxyModel(mRoomWrapper->roomId());
        connect(model, &UsersForRoomFilterProxyModel::rowsInserted, this, &UsersInRoomFlowWidget::updateList);
        connect(model, &UsersForRoomFilterProxyModel::rowsRemoved, this, &UsersInRoomFlowWidget::updateList);
        connect(model, &UsersForRoomFilterProxyModel::dataChanged, this, &UsersInRoomFlowWidget::updateList);
        connect(model, &UsersForRoomFilterProxyModel::modelReset, this, &UsersInRoomFlowWidget::updateList);
        connect(model, &UsersForRoomFilterProxyModel::hasFullListChanged, this, &UsersInRoomFlowWidget::updateList);
        updateList();
    } else {
        mFlowLayout->clearAndDeleteWidgets();
    }
}

void UsersInRoomFlowWidget::showEvent(QShowEvent *event)
{
    updateList();
    QWidget::showEvent(event);
}

void UsersInRoomFlowWidget::updateList()
{
    if (isVisible()) {
        const auto model = Ruqola::self()->rocketChatAccount()->usersForRoomFilterProxyModel(mRoomWrapper->roomId());
        const auto count = model->rowCount();
        mFlowLayout->clearAndDeleteWidgets();
        for (int i = 0; i < count; ++i) {
            const auto roomModelIndex = model->index(i, 0);
            const QString userName = roomModelIndex.data(UsersForRoomModel::UsersForRoomRoles::DisplayName).toString();
            const QString iconStatus = roomModelIndex.data(UsersForRoomModel::UsersForRoomRoles::IconStatus).toString();
            const QString userId = roomModelIndex.data(UsersForRoomModel::UsersForRoomRoles::UserId).toString();
            UsersInRoomLabel *userLabel = new UsersInRoomLabel(this);
            userLabel->setUserName(userName);
            userLabel->setIconStatus(iconStatus);
            userLabel->setUserId(userId);
            userLabel->setRoomWrapper(mRoomWrapper);
            mFlowLayout->addWidget(userLabel);
        }
        if (!model->hasFullList()) {
            QLabel *loadingMoreLabel = new QLabel(QStringLiteral("<a href=\"loadmoreelement\">%1</a>").arg(i18n("(Click here for Loading more...)")), this);
            loadingMoreLabel->setTextFormat(Qt::RichText);
            loadingMoreLabel->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(loadingMoreLabel, &QLabel::linkActivated, this, &UsersInRoomFlowWidget::loadMoreUsersAttachment);
            mFlowLayout->addWidget(loadingMoreLabel);
        }
    }
}

void UsersInRoomFlowWidget::loadMoreUsersAttachment()
{
    Ruqola::self()->rocketChatAccount()->loadMoreUsersInRoom(mRoomWrapper->roomId(), mRoomWrapper->channelType());
}
