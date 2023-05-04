/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorroomseditwidget.h"
#include "dialogs/messagetexteditor.h"
#include "dialogs/roomavatarwidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
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
    , mOwnerName(new QLabel(this))
    , mRoomAvatarWidget(new RoomAvatarWidget(this))
    , mTopic(new MessageTextEditor(this))
    , mAnnouncement(new MessageTextEditor(this))
    , mDescription(new MessageTextEditor(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mRoomAvatarWidget->setObjectName(QStringLiteral("mRoomAvatarWidget"));
    mainLayout->addWidget(mRoomAvatarWidget);

    mOwnerName->setObjectName(QStringLiteral("mOwnerName"));
    mainLayout->addRow(i18n("Room's Creator:"), mOwnerName);

    mLineEdit->setObjectName(QStringLiteral("mLineEdit"));
    mainLayout->addRow(i18n("Name:"), mLineEdit);
    new LineEditCatchReturnKey(mLineEdit, this);

    mTopic->setObjectName(QStringLiteral("mComment"));
    mainLayout->addRow(i18n("Comment:"), mTopic);

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

    mainLayout->addWidget(mReadOnlyCheckBox);
    mainLayout->addWidget(mPrivateCheckBox);
    mainLayout->addWidget(mDefaultCheckBox);
    mainLayout->addWidget(mFavoriteCheckBox);
    mainLayout->addWidget(mFeaturedCheckBox);
    mainLayout->addWidget(mArchivedCheckBox);
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
    mArchivedCheckBox->setChecked(newRoomEditInfo.archived);

    mTopic->setPlainText(newRoomEditInfo.topic);
    mAnnouncement->setPlainText(newRoomEditInfo.announcement);
    mDescription->setPlainText(newRoomEditInfo.description);

    mOwnerName->setText(newRoomEditInfo.userOwnerName);
    // TODO mRoomAvatarWidget
}

AdministratorRoomsEditBaseWidget::RoomEditInfo AdministratorRoomsEditWidget::roomEditInfo() const
{
    AdministratorRoomsEditBaseWidget::RoomEditInfo info;
    info.featured = mFeaturedCheckBox->isChecked();
    info.defaultRoom = mDefaultCheckBox->isChecked();
    info.privateRoom = mPrivateCheckBox->isChecked();

    info.archived = mArchivedCheckBox->isChecked();

    info.name = mLineEdit->text();
    info.favorite = mFavoriteCheckBox->isChecked();

    info.readOnly = mReadOnlyCheckBox->isChecked();

    info.topic = mTopic->toPlainText();
    info.announcement = mAnnouncement->toPlainText();
    info.description = mDescription->toPlainText();

    // TODO mRoomAvatarWidget
    return info;
}
