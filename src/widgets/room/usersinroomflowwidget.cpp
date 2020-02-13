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
#include "model/usersforroommodel.h"
#include "model/usersforroomfilterproxymodel.h"
#include <KLocalizedString>
#include <QLabel>

UsersInRoomFlowWidget::UsersInRoomFlowWidget(QWidget *parent)
    : QWidget(parent)
{
    mFlowLayout = new FlowLayout(this);
}

UsersInRoomFlowWidget::~UsersInRoomFlowWidget()
{

}

void UsersInRoomFlowWidget::setRoomId(const QString &roomId)
{
    if (mRoomId != roomId) {
        mRoomId = roomId;
        updateList();
    }
}

void UsersInRoomFlowWidget::updateList()
{
    if (/*isVisible()*/1) {
        const auto model = Ruqola::self()->rocketChatAccount()->usersForRoomFilterProxyModel(mRoomId);
        const auto count = model->rowCount();
        mFlowLayout->clearAndDeleteWidgets();
        for (int i = 0; i < count; ++i) {
            const auto roomModelIndex = model->index(i, 0);
            const auto userName = roomModelIndex.data(UsersForRoomModel::UsersForRoomRoles::UserName).toString();
            UsersInRoomLabel *userLabel = new UsersInRoomLabel(this);
            userLabel->setUserName(userName);
            mFlowLayout->addWidget(userLabel);
        }
        if (!model->hasFullList()) {
            mFlowLayout->addWidget(new QLabel(i18n("(load More elements)")));
        }
    }
}
