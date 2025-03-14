/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorroomseditwidget.h"
#include "dialogs/messagetexteditor.h"
#include "dialogs/roomavatarwidget.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

AdministratorRoomsEditWidget::AdministratorRoomsEditWidget(QWidget *parent)
    : AdministratorRoomsEditBaseWidget{parent}
    , mLineEdit(new QLineEdit(this))
    , mDefaultCheckBox(new QCheckBox(i18nc("@option:check", "Default"), this))
    , mFavoriteCheckBox(new QCheckBox(i18nc("@option:check", "Favorite"), this))
    , mFeaturedCheckBox(new QCheckBox(i18nc("@option:check", "Featured"), this))
    , mPrivateCheckBox(new QCheckBox(i18nc("@option:check", "Private"), this))
    , mReadOnlyCheckBox(new QCheckBox(i18nc("@option:check", "Read-Only"), this))
    , mArchivedCheckBox(new QCheckBox(i18nc("@option:check", "Archived"), this))
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
    KLineEditEventHandler::catchReturnKey(mLineEdit);

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
    auto label = new QLabel(i18nc("@label:textbox", "Only authorized users can write new messages"), this);
    mainLayout->addWidget(label);

    mainLayout->addWidget(mPrivateCheckBox);
    label = new QLabel(i18nc("@label:textbox", "Just invited people can access this channel."), this);
    mainLayout->addWidget(label);

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

#include "moc_administratorroomseditwidget.cpp"
