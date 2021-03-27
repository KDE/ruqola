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

#include "channelinfoeditablewidget.h"
#include "channelinfoprunewidget.h"
#include "misc/systemmessagescombobox.h"
#include "rocketchataccount.h"
#include "roomavatarwidget.h"
#include "ruqola.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <KPasswordLineEdit>
#include <KSeparator>
#include <QCheckBox>
#include <QFormLayout>
#include <QPushButton>

ChannelInfoEditableWidget::ChannelInfoEditableWidget(QWidget *parent)
    : QWidget(parent)
    , mName(new QLineEdit(this))
    , mComment(new QLineEdit(this))
    , mAnnouncement(new QLineEdit(this))
    , mDescription(new QLineEdit(this))
    , mPasswordLineEdit(new KPasswordLineEdit(this))
    , mReadOnly(new QCheckBox(this))
    , mArchive(new QCheckBox(this))
    , mPrivate(new QCheckBox(this))
    , mEncrypted(new QCheckBox(this))
    , mDeleteChannel(new QPushButton(QIcon::fromTheme(QStringLiteral("edit-delete-shred")), i18n("Delete"), this))
    , mChannelInfoPruneWidget(new ChannelInfoPruneWidget(this))
    , mSystemMessageCombox(new SystemMessagesComboBox(this))
    , mRoomAvatarWidget(new RoomAvatarWidget(this))
{
    auto layout = new QFormLayout(this);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins({});

    mRoomAvatarWidget->setObjectName(QStringLiteral("mRoomAvatarWidget"));
    layout->addRow(QStringLiteral(" "), mRoomAvatarWidget);
    QString str = i18n("Name:");
    mName->setObjectName(QStringLiteral("mName"));
    layout->addRow(str, mName);

    mComment->setObjectName(QStringLiteral("mComment"));
    // mComment->setAllowEmptyText(true);
    str = i18n("Comment:");
    layout->addRow(str, mComment);

    mAnnouncement->setObjectName(QStringLiteral("mAnnouncement"));
    // mAnnouncement->setAllowEmptyText(true);
    str = i18n("Announcement:");
    layout->addRow(str, mAnnouncement);

    mDescription->setObjectName(QStringLiteral("mDescription"));
    str = i18n("Description:");
    // mDescription->setLabelText(str);

    layout->addRow(str, mDescription);

    // Show it if room is not private
    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
    layout->addRow(i18n("Password:"), mPasswordLineEdit);

    mReadOnly->setObjectName(QStringLiteral("mReadOnly"));
    layout->addRow(i18n("ReadOnly:"), mReadOnly);

    mArchive->setObjectName(QStringLiteral("mArchive"));
    layout->addRow(i18n("Archive:"), mArchive);
    const bool canArchiveOrUnarchive = (Ruqola::self()->rocketChatAccount()->hasPermission(QStringLiteral("archive-room"))
                                        || Ruqola::self()->rocketChatAccount()->hasPermission(QStringLiteral("unarchive-room")));
    mArchive->setEnabled(canArchiveOrUnarchive);
    connect(mArchive, &QCheckBox::clicked, this, [this](bool checked) {
        const QString text = checked ? i18n("Do you want to archive this room?") : i18n("Do you want to unarchive this room?");
        const QString title = checked ? i18n("Archive Channel") : i18n("Unarchive Channel");
        if (KMessageBox::Yes == KMessageBox::questionYesNo(this, text, title)) {
            // Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoom->roomId(), RocketChatAccount::Archive, checked, mRoom->channelType());
        }
    });

    mPrivate->setObjectName(QStringLiteral("mPrivate"));
    layout->addRow(i18n("Private:"), mPrivate);

    mEncrypted->setObjectName(QStringLiteral("mEncrypted"));
    layout->addRow(i18n("Encrypted:"), mEncrypted);
    mEncryptedLabel = layout->labelForField(mEncrypted);

    mSystemMessageCombox->setObjectName(QStringLiteral("mSystemMessageCombox"));
    layout->addRow(i18n("Hide System Messages:"), mSystemMessageCombox);

    mChannelInfoPruneWidget->setObjectName(QStringLiteral("mChannelInfoPruneWidget"));
    layout->addRow(mChannelInfoPruneWidget);
    auto separator = new KSeparator(this);
    separator->setObjectName(QStringLiteral("separator"));
    layout->addWidget(separator);

    mDeleteChannel->setObjectName(QStringLiteral("mDeleteChannel"));
    layout->addRow(QStringLiteral(" "), mDeleteChannel);
    connect(mDeleteChannel, &QPushButton::clicked, this, [this]() {
        if (KMessageBox::Yes == KMessageBox::questionYesNo(this, i18n("Do you want to delete this room?"), i18n("Delete Room"))) {
            Ruqola::self()->rocketChatAccount()->eraseRoom(mRoom->roomId(), mRoom->channelType());
            Q_EMIT channelDeleted();
        }
    });
}

ChannelInfoEditableWidget::~ChannelInfoEditableWidget()
{
}

void ChannelInfoEditableWidget::setRoom(Room *room)
{
    mRoom = room;
}

RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo ChannelInfoEditableWidget::saveRoomSettingsInfo() const
{
    RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo info;
    if (mName->text().isEmpty()) {
        return info;
    }

    info.roomId = mRoom->roomId();
    info.joinCode = mPasswordLineEdit->password();
    if (mRoom->name() != mName->text()) {
        info.roomName = mName->text();
        info.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::RoomName;
    }
    if (mRoom->announcement() != mAnnouncement->text()) {
        info.roomAnnouncement = mAnnouncement->text();
        info.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::RoomAnnouncement;
    }
    if (mRoom->description() != mDescription->text()) {
        info.roomDescription = mDescription->text();
        info.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::RoomDescription;
    }
    if (mRoom->topic() != mComment->text()) {
        info.roomTopic = mComment->text();
        info.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::RoomTopic;
    }
    if (mRoom->readOnly() != mReadOnly->isChecked()) {
        info.readOnly = mReadOnly->isChecked();
        info.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::ReadOnly;
    }
    if (mRoom->displaySystemMessageTypes() != mSystemMessageCombox->systemMessagesSelected()) {
        info.systemMessages = mSystemMessageCombox->systemMessagesSelected();
        info.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::SystemMessages;
    }
    if (mRoom->encrypted() != mEncrypted->isChecked()) {
        info.encrypted = mEncrypted->isChecked();
        info.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::Encrypted;
    }

    if ((mRoom->channelType() == QStringLiteral("p")) != mPrivate->isChecked()) {
        info.roomType = mPrivate->isChecked() ? QStringLiteral("p") : QStringLiteral("c");
        info.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::RoomType;
    }

    if (hasRetentionPermission()) {
        mChannelInfoPruneWidget->saveRoomSettingsInfo(info, mRoom);
    }
    if (mRoomAvatarWidget->wasChanged()) {
        info.roomAvatarPath = mRoomAvatarWidget->roomAvatarPath();
        info.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::RoomAvatar;
    }
    qDebug() << " info " << info;
    // TODO
    //    mArchive->setChecked(mRoom->archived());
    return info;
}

void ChannelInfoEditableWidget::updateEditableChannelInfo()
{
    mName->setText(mRoom->displayFName());
    mComment->setText(mRoom->topic());
    mAnnouncement->setText(mRoom->announcement());
    mDescription->setText(mRoom->description());
    mReadOnly->setChecked(mRoom->readOnly());
    mArchive->setChecked(mRoom->archived());
    mPrivate->setChecked(mRoom->channelType() == QStringLiteral("p"));
    mEncrypted->setVisible(mRoom->encryptedEnabled());
    mEncrypted->setChecked(mRoom->encrypted());
    mEncryptedLabel->setVisible(mRoom->encryptedEnabled());
    mSystemMessageCombox->setMessagesSystem(mRoom->displaySystemMessageTypes());
    joinCodeChanged();
}

void ChannelInfoEditableWidget::joinCodeChanged()
{
    mPasswordLineEdit->lineEdit()->setPlaceholderText(mRoom->joinCodeRequired() ? i18n("This Room has a password") : i18n("Add password"));
}

void ChannelInfoEditableWidget::connectEditableWidget()
{
    connect(mRoom, &Room::announcementChanged, this, [this]() {
        mAnnouncement->setText(mRoom->announcement());
    });
    connect(mRoom, &Room::topicChanged, this, [this]() {
        mComment->setText(mRoom->topic());
    });
    connect(mRoom, &Room::fnameChanged, this, [this]() {
        mName->setText(mRoom->fName());
        Q_EMIT fnameChanged(mRoom->fName());
    });
    connect(mRoom, &Room::descriptionChanged, this, [this]() {
        mDescription->setText(mRoom->description());
    });
    connect(mRoom, &Room::readOnlyChanged, this, [this]() {
        mReadOnly->setChecked(mRoom->readOnly());
    });
    connect(mRoom, &Room::archivedChanged, this, [this]() {
        mArchive->setChecked(mRoom->archived());
    });
    connect(mRoom, &Room::joinCodeRequiredChanged, this, [this]() {
        joinCodeChanged();
    });
    connect(mRoom, &Room::channelTypeChanged, this, [this]() {
        mPrivate->setChecked(mRoom->channelType() == QStringLiteral("p"));
    });
    // TODO react when we change settings
    //    connect(mReadOnly, &QCheckBox::clicked, this, [this](bool checked) {
    //        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoom->roomId(), RocketChatAccount::ReadOnly, checked, mRoom->channelType());
    //    });
    //    connect(mArchive, &QCheckBox::clicked, this, [this](bool checked) {
    //        if (KMessageBox::Yes
    //            == KMessageBox::questionYesNo(this,
    //                                          checked ? i18n("Do you want to archive this room?") : i18n("Do you want to unarchive this room?"),
    //                                          i18n("Archive room"))) {
    //            Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoom->roomId(), RocketChatAccount::Archive, checked, mRoom->channelType());
    //        }
    //    });
}

void ChannelInfoEditableWidget::updateRetentionValue()
{
    if (!mChannelInfoPruneWidget->isHidden()) {
        mChannelInfoPruneWidget->setRetentionInfo(mRoom->retentionInfo());
    }
}

void ChannelInfoEditableWidget::updateUiFromPermission()
{
    mChannelInfoPruneWidget->setHidden(!hasRetentionPermission());
}

bool ChannelInfoEditableWidget::hasRetentionPermission() const
{
    return Ruqola::self()->rocketChatAccount()->hasPermission(QStringLiteral("edit-room-retention-policy"));
}
