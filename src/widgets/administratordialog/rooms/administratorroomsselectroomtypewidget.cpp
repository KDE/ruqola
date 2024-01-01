/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    , mDiscussionRooms(new QCheckBox(i18n("Discussions"), this))
    , mTeamRooms(new QCheckBox(i18n("Teams"), this))
    , mOmniChannel(new QCheckBox(i18n("Omnichannel"), this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mDirectRooms->setObjectName(QStringLiteral("mDirectRooms"));
    mPublicRooms->setObjectName(QStringLiteral("mPublicRooms"));
    mPrivateRooms->setObjectName(QStringLiteral("mPrivateRooms"));
    mDiscussionRooms->setObjectName(QStringLiteral("mDiscussionRooms"));
    mTeamRooms->setObjectName(QStringLiteral("mTeamRooms"));
    mOmniChannel->setObjectName(QStringLiteral("mOmniChannel"));

    mainLayout->addWidget(mDirectRooms);
    mainLayout->addWidget(mPublicRooms);
    mainLayout->addWidget(mPrivateRooms);
    mainLayout->addWidget(mDiscussionRooms);
    mainLayout->addWidget(mTeamRooms);
    mainLayout->addWidget(mOmniChannel);
    mainLayout->addStretch(0);
    connect(mDirectRooms, &QCheckBox::clicked, this, &AdministratorRoomsSelectRoomTypeWidget::slotFilterChanged);
    connect(mPublicRooms, &QCheckBox::clicked, this, &AdministratorRoomsSelectRoomTypeWidget::slotFilterChanged);
    connect(mPrivateRooms, &QCheckBox::clicked, this, &AdministratorRoomsSelectRoomTypeWidget::slotFilterChanged);
    connect(mDiscussionRooms, &QCheckBox::clicked, this, &AdministratorRoomsSelectRoomTypeWidget::slotFilterChanged);
    connect(mTeamRooms, &QCheckBox::clicked, this, &AdministratorRoomsSelectRoomTypeWidget::slotFilterChanged);
    connect(mOmniChannel, &QCheckBox::clicked, this, &AdministratorRoomsSelectRoomTypeWidget::slotFilterChanged);
}

AdministratorRoomsSelectRoomTypeWidget::~AdministratorRoomsSelectRoomTypeWidget() = default;

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
    if (mOmniChannel->isChecked()) {
        filters |= AdminRoomsFilterProxyModel::FilterRoom::Omnichannel;
    }
    if (mTeamRooms->isChecked()) {
        filters |= AdminRoomsFilterProxyModel::FilterRoom::TeamRooms;
    }
    if (filters == AdminRoomsFilterProxyModel::FilterRoom::None) {
        filters = AdminRoomsFilterProxyModel::FilterRoom::All;
    }
    Q_EMIT filterChanged(filters);
}

#include "moc_administratorroomsselectroomtypewidget.cpp"
