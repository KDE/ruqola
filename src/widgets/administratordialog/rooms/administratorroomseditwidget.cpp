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
#include "dialogs/messagetexteditor.h"
#include "dialogs/roomavatarwidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>

AdministratorRoomsEditWidget::AdministratorRoomsEditWidget(QWidget *parent)
    : AdministratorRoomsEditBaseWidget{parent}
    , mLineEdit(new QLineEdit(this))
    , mDefaultCheckBox(new QCheckBox(i18n("Default"), this))
    , mFavoriteCheckBox(new QCheckBox(i18n("Favorite"), this))
    , mFeaturedCheckBox(new QCheckBox(i18n("Featured"), this))
    , mPrivateCheckBox(new QCheckBox(i18n("Private"), this))
    , mReadOnlyCheckBox(new QCheckBox(i18n("Read-Only"), this))
    , mArchivedCheckBox(new QCheckBox(i18n("Archived"), this))
    , mRoomAvatarWidget(new RoomAvatarWidget(this))
    , mComment(new MessageTextEditor(this))
    , mAnnouncement(new MessageTextEditor(this))
    , mDescription(new MessageTextEditor(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mRoomAvatarWidget->setObjectName(QStringLiteral("mRoomAvatarWidget"));
    mainLayout->addWidget(mRoomAvatarWidget);

    mLineEdit->setObjectName(QStringLiteral("mLineEdit"));
    mainLayout->addRow(i18n("Name:"), mLineEdit);
    new LineEditCatchReturnKey(mLineEdit, this);

    mComment->setObjectName(QStringLiteral("mComment"));
    mainLayout->addRow(i18n("Comment:"), mComment);

    mAnnouncement->setObjectName(QStringLiteral("mAnnouncement"));
    mainLayout->addRow(i18n("Announcement:"), mAnnouncement);

    mDescription->setObjectName(QStringLiteral("mDescription"));
    mainLayout->addRow(i18n("Description:"), mDescription);

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

void AdministratorRoomsEditWidget::setRoomEditInfo(const RoomEditInfo &newRoomEditInfo)
{
    mLineEdit->setText(newRoomEditInfo.name);
    mFeaturedCheckBox->setChecked(newRoomEditInfo.featured);
    mDefaultCheckBox->setChecked(newRoomEditInfo.defaultRoom);
    mFavoriteCheckBox->setChecked(newRoomEditInfo.favorite);
    mReadOnlyCheckBox->setChecked(newRoomEditInfo.readOnly);
    mPrivateCheckBox->setChecked(newRoomEditInfo.privateRoom);

    mComment->setPlainText(newRoomEditInfo.comment); // TODO Plain text ???
    mAnnouncement->setPlainText(newRoomEditInfo.announcement); // TODO Plain text ???
    mDescription->setPlainText(newRoomEditInfo.description); // TODO Plain text ???

    // TODO mRoomAvatarWidget mArchivedCheckBox
}

AdministratorRoomsEditBaseWidget::RoomEditInfo AdministratorRoomsEditWidget::roomEditInfo() const
{
    AdministratorRoomsEditBaseWidget::RoomEditInfo info;
    info.featured = mFeaturedCheckBox->isChecked();
    info.defaultRoom = mDefaultCheckBox->isChecked();
    info.privateRoom = mPrivateCheckBox->isChecked();

    info.name = mLineEdit->text();
    info.favorite = mFavoriteCheckBox->isChecked();

    info.readOnly = mReadOnlyCheckBox->isChecked();

    info.comment = mComment->toPlainText(); // TODO Plain text ???
    info.announcement = mAnnouncement->toPlainText(); // TODO Plain text ???
    info.description = mDescription->toPlainText(); // TODO Plain text ???

    // TODO
    return info;
}
