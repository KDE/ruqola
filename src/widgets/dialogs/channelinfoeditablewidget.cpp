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
{
    auto layout = new QFormLayout(this);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins({});

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
            Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoom->roomId(), RocketChatAccount::Archive, checked, mRoom->channelType());
        }
    });

    mPrivate->setObjectName(QStringLiteral("mPrivate"));
    layout->addRow(i18n("Private:"), mPrivate);

    mEncrypted->setObjectName(QStringLiteral("mEncrypted"));
    layout->addRow(i18n("Encrypted:"), mEncrypted);
    mEncryptedLabel = layout->labelForField(mEncrypted);

    mSystemMessageCombox->setObjectName(QStringLiteral("mSystemMessageCombox"));
    layout->addRow(i18n("Hide System Messages:"), mSystemMessageCombox);
    //    connect(mSystemMessageCombox, &SystemMessagesComboBox::settingsChanged, this, [this]() {
    //        auto *rcAccount = Ruqola::self()->rocketChatAccount();
    //        auto saveRoomSettingsJob = new RocketChatRestApi::SaveRoomSettingsJob(this);
    //        RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo info;
    //        info.mSettingsWillBeChanged = info.mSettingsWillBeChanged | RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::SystemMessages;
    //        info.roomId = mRoom->roomId();
    //        qDebug() << " info.roomId" << info.roomId;
    //        info.systemMessages = mSystemMessageCombox->systemMessagesSelected();
    //        saveRoomSettingsJob->setSaveRoomSettingsInfo(info);
    //        rcAccount->restApi()->initializeRestApiJob(saveRoomSettingsJob);
    //        // connect(saveRoomSettingsJob, &RocketChatRestApi::SaveRoomSettingsJob::saveRoomSettingsDone, this,
    //        &AdministratorRoomsWidget::slotAdminRoomDone); if (!saveRoomSettingsJob->start()) {
    //            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start saveRoomSettingsJob";
    //        }
    //    });

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

RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo ChannelInfoEditableWidget::saveRoomSettingsInfo() const
{
    // TODO
    return {};
}
