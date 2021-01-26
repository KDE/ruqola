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

#include "administratorroomsselectroomtypewidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QHBoxLayout>

AdministratorRoomsSelectRoomTypeWidget::AdministratorRoomsSelectRoomTypeWidget(QWidget *parent)
    : QWidget(parent)
    , mDirectRooms(new QCheckBox(i18n("Direct"), this))
    , mPublicRooms(new QCheckBox(i18n("Public"), this))
    , mPrivateRooms(new QCheckBox(i18n("Private"), this))
    , mDiscussionRooms(new QCheckBox(i18n("Discussion"), this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mDirectRooms->setObjectName(QStringLiteral("mDirectRooms"));
    mPublicRooms->setObjectName(QStringLiteral("mPublicRooms"));
    mPrivateRooms->setObjectName(QStringLiteral("mPrivateRooms"));
    mDiscussionRooms->setObjectName(QStringLiteral("mDiscussionRooms"));

    mainLayout->addWidget(mDirectRooms);
    mainLayout->addWidget(mPublicRooms);
    mainLayout->addWidget(mPrivateRooms);
    mainLayout->addWidget(mDiscussionRooms);
    mainLayout->addStretch(0);
    connect(mDirectRooms, &QCheckBox::clicked, this, &AdministratorRoomsSelectRoomTypeWidget::slotFilterChanged);
    connect(mPublicRooms, &QCheckBox::clicked, this, &AdministratorRoomsSelectRoomTypeWidget::slotFilterChanged);
    connect(mPrivateRooms, &QCheckBox::clicked, this, &AdministratorRoomsSelectRoomTypeWidget::slotFilterChanged);
    connect(mDiscussionRooms, &QCheckBox::clicked, this, &AdministratorRoomsSelectRoomTypeWidget::slotFilterChanged);
}

AdministratorRoomsSelectRoomTypeWidget::~AdministratorRoomsSelectRoomTypeWidget()
{
}

void AdministratorRoomsSelectRoomTypeWidget::slotFilterChanged()
{
    AdminRoomsFilterProxyModel::FilterRooms filters;
    filters |= AdminRoomsFilterProxyModel::FilterRoom::None;
    if (mDirectRooms->isChecked()) {
        filters |= AdminRoomsFilterProxyModel::FilterRoom::DirectRooms;
    }
    if (mPublicRooms->isChecked()) {
        filters |= AdminRoomsFilterProxyModel::FilterRoom::PublicRooms;
    }
    if (mPrivateRooms->isChecked()) {
        filters |= AdminRoomsFilterProxyModel::FilterRoom::PrivateRooms;
    }
    if (mDiscussionRooms->isChecked()) {
        filters |= AdminRoomsFilterProxyModel::FilterRoom::DiscussionRooms;
    }
    if (filters == AdminRoomsFilterProxyModel::FilterRoom::None) {
        filters = AdminRoomsFilterProxyModel::FilterRoom::All;
    }
    Q_EMIT filterChanged(filters);
}
