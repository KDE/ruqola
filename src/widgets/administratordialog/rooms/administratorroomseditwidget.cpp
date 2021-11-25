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

#include "administratorroomseditwidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>

AdministratorRoomsEditWidget::AdministratorRoomsEditWidget(QWidget *parent)
    : QWidget{parent}
    , mLineEdit(new QLineEdit(this))
    , mDefaultCheckBox(new QCheckBox(i18n("Default"), this))
    , mFavoriteCheckBox(new QCheckBox(i18n("Favorite"), this))
    , mFeaturedCheckBox(new QCheckBox(i18n("Featured"), this))
    , mPrivateCheckBox(new QCheckBox(i18n("Private"), this))
    , mReadOnlyCheckBox(new QCheckBox(i18n("Read-Only"), this))
    , mArchivedCheckBox(new QCheckBox(i18n("Archived"), this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mLineEdit->setObjectName(QStringLiteral("mLineEdit"));
    mainLayout->addRow(i18n("Name:"), mLineEdit);

    mDefaultCheckBox->setObjectName(QStringLiteral("mDefaultCheckBox"));
    mFavoriteCheckBox->setObjectName(QStringLiteral("mFavoriteCheckBox"));
    mFeaturedCheckBox->setObjectName(QStringLiteral("mFeaturedCheckBox"));
    mPrivateCheckBox->setObjectName(QStringLiteral("mPrivateCheckBox"));
    mReadOnlyCheckBox->setObjectName(QStringLiteral("mReadOnlyCheckBox"));
    mArchivedCheckBox->setObjectName(QStringLiteral("mArchivedCheckBox"));

    mainLayout->addRow(mReadOnlyCheckBox);
    mainLayout->addRow(mPrivateCheckBox);
    mainLayout->addRow(mDefaultCheckBox);
    mainLayout->addRow(mFavoriteCheckBox);
    mainLayout->addRow(mFeaturedCheckBox);
    mainLayout->addRow(mArchivedCheckBox);
}

AdministratorRoomsEditWidget::~AdministratorRoomsEditWidget() = default;
