/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratordirectroomseditwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>

AdministratorDirectRoomsEditWidget::AdministratorDirectRoomsEditWidget(QWidget *parent)
    : AdministratorRoomsEditBaseWidget{parent}
    , mLineEdit(new QLineEdit(this))
    , mDefaultCheckBox(new QCheckBox(i18nc("@option:check", "Default"), this))
    , mFavoriteCheckBox(new QCheckBox(i18nc("@option:check", "Favorite"), this))
    , mFeaturedCheckBox(new QCheckBox(i18nc("@option:check", "Featured"), this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mLineEdit->setObjectName(u"mLineEdit"_s);
    mainLayout->addRow(i18n("Name:"), mLineEdit);
    mLineEdit->setReadOnly(true);

    mDefaultCheckBox->setObjectName(u"mDefaultCheckBox"_s);
    mFavoriteCheckBox->setObjectName(u"mFavoriteCheckBox"_s);
    mFeaturedCheckBox->setObjectName(u"mFeaturedCheckBox"_s);

    mainLayout->addWidget(mDefaultCheckBox);
    mainLayout->addWidget(mFavoriteCheckBox);
    mainLayout->addWidget(mFeaturedCheckBox);
}

AdministratorDirectRoomsEditWidget::~AdministratorDirectRoomsEditWidget() = default;

AdministratorRoomsEditBaseWidget::RoomEditInfo AdministratorDirectRoomsEditWidget::roomEditInfo() const
{
    AdministratorRoomsEditBaseWidget::RoomEditInfo info;
    info.name = mLineEdit->text();
    info.featured = mFeaturedCheckBox->isChecked();
    info.defaultRoom = mDefaultCheckBox->isChecked();
    info.favorite = mFavoriteCheckBox->isChecked();
    return info;
}

void AdministratorDirectRoomsEditWidget::setRoomEditInfo(const RoomEditInfo &newRoomEditInfo)
{
    mLineEdit->setText(newRoomEditInfo.name);
    mFeaturedCheckBox->setChecked(newRoomEditInfo.featured);
    mDefaultCheckBox->setChecked(newRoomEditInfo.defaultRoom);
    mFavoriteCheckBox->setChecked(newRoomEditInfo.favorite);
}

#include "moc_administratordirectroomseditwidget.cpp"
