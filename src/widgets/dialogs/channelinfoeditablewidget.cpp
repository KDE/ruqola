/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinfoeditablewidget.h"
#include "channelinfoprunewidget.h"
#include "connection.h"
#include "messagetexteditor.h"
#include "misc/systemmessagescombobox.h"
#include "rocketchataccount.h"
#include "roomavatarwidget.h"
#include "ruqolawidgets_debug.h"
#include "teams/teamdeletejob.h"
#include "teams/teamroom.h"
#include "teams/teamselectdeletedroomdialog.h"
#include "teams/teamslistroomsjob.h"
#include <KAuthorized>
#include <KLocalizedString>
#include <KMessageBox>
#include <KPasswordLineEdit>
#include <KSeparator>
#include <QCheckBox>
#include <QFormLayout>
#include <QPointer>
#include <QPushButton>

ChannelInfoEditableWidget::ChannelInfoEditableWidget(Room *room, RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mRoom(room)
    , mName(new QLineEdit(this))
    , mComment(new MessageTextEditor(this))
    , mAnnouncement(new MessageTextEditor(this))
    , mDescription(new MessageTextEditor(this))
    , mPasswordLineEdit(new KPasswordLineEdit(this))
    , mReadOnly(new QCheckBox(this))
    , mArchive(new QCheckBox(this))
    , mPrivate(new QCheckBox(this))
    , mEncrypted(new QCheckBox(this))
    , mSystemMessageCombox(new SystemMessagesComboBox(this))
    , mDeleteChannel(new QPushButton(QIcon::fromTheme(QStringLiteral("edit-delete-shred")), i18nc("@action:button", "Delete"), this))
    , mChannelInfoPruneWidget(new ChannelInfoPruneWidget(this))
    , mRoomAvatarWidget(new RoomAvatarWidget(this))
    , mRocketChatAccount(account)
{
    auto layout = new QFormLayout(this);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins({});

    mRoomAvatarWidget->setObjectName(QStringLiteral("mRoomAvatarWidget"));
    layout->addRow(QStringLiteral(" "), mRoomAvatarWidget);
    QString str = i18n("Name:");
    mName->setObjectName(QStringLiteral("mName"));
    layout->addRow(str, mName);
    connect(mName, &QLineEdit::textChanged, this, [this](const QString &str) {
        Q_EMIT roomNameValid(!str.trimmed().isEmpty());
    });

    mComment->setObjectName(QStringLiteral("mComment"));
    str = i18n("Comment:");
    layout->addRow(str, mComment);

    mAnnouncement->setObjectName(QStringLiteral("mAnnouncement"));
    str = i18n("Announcement:");
    layout->addRow(str, mAnnouncement);

    mDescription->setObjectName(QStringLiteral("mDescription"));
    str = i18n("Description:");

    layout->addRow(str, mDescription);

    // Show it if room is not private
    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
    mPasswordLineEdit->setRevealPasswordMode(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")) ? KPassword::RevealMode::OnlyNew
                                                                                                                : KPassword::RevealMode::Never);
    layout->addRow(i18n("Password:"), mPasswordLineEdit);

    mReadOnly->setObjectName(QStringLiteral("mReadOnly"));
    layout->addRow(i18n("ReadOnly:"), mReadOnly);
    mReadOnly->setToolTip(i18nc("@info:tooltip", "Messages are end-to-end encrypted, search will not work and notifications may not show message content"));

    mArchive->setObjectName(QStringLiteral("mArchive"));
    layout->addRow(i18n("Archive:"), mArchive);
    const bool canArchiveOrUnarchive = mRocketChatAccount
        && (mRocketChatAccount->hasPermission(QStringLiteral("archive-room")) || mRocketChatAccount->hasPermission(QStringLiteral("unarchive-room")));
    mArchive->setEnabled(canArchiveOrUnarchive);
    connect(mArchive, &QCheckBox::clicked, this, [this](bool checked) {
        const QString text = checked ? i18n("Do you want to archive this room?") : i18n("Do you want to unarchive this room?");
        const QString title = checked ? i18nc("@title", "Archive Channel") : i18nc("@title", "Unarchive Channel");
        if (KMessageBox::ButtonCode::PrimaryAction == KMessageBox::questionTwoActions(this, text, title, KStandardGuiItem::ok(), KStandardGuiItem::cancel())) {
            // mRocketChatAccount->changeChannelSettings(mRoom->roomId(), RocketChatAccount::Archive, checked, mRoom->channelType());
        }
    });

    mPrivate->setObjectName(QStringLiteral("mPrivate"));
    layout->addRow(i18n("Private:"), mPrivate);
    mPrivate->setToolTip(i18nc("@info:tooltip", "People can only join by being invited"));

    mEncrypted->setObjectName(QStringLiteral("mEncrypted"));
    layout->addRow(i18n("Encrypted:"), mEncrypted);
    mEncryptedLabel = layout->labelForField(mEncrypted);
    mEncryptedLabel->setToolTip(
        i18nc("@info:tooltip", "Messages are end-to-end encrypted, search will not work and notifications may not show message content"));

    mEncrypted->setEnabled(false);

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
        if (mRoom->teamInfo().mainTeam()) {
            if (KMessageBox::ButtonCode::PrimaryAction
                == KMessageBox::questionTwoActions(this,
                                                   i18n("Do you want to delete this Team?"),
                                                   i18nc("@title", "Delete Team"),
                                                   KStandardGuiItem::del(),
                                                   KStandardGuiItem::cancel())) {
                selectRoomToDelete(mRoom->teamInfo().teamId());
            }
        } else {
            if (KMessageBox::ButtonCode::PrimaryAction
                == KMessageBox::questionTwoActions(this,
                                                   i18n("Do you want to delete this room?"),
                                                   i18nc("@title", "Delete Room"),
                                                   KStandardGuiItem::del(),
                                                   KStandardGuiItem::cancel())) {
                mRocketChatAccount->eraseRoom(mRoom->roomId(), mRoom->channelType());
                Q_EMIT channelDeleted();
            }
        }
    });

    connect(mPrivate, &QCheckBox::clicked, this, [this](bool clicked) {
        mEncrypted->setEnabled(clicked);
    });
}

ChannelInfoEditableWidget::~ChannelInfoEditableWidget() = default;

void ChannelInfoEditableWidget::selectRoomToDelete(const QByteArray &teamId)
{
    auto job = new RocketChatRestApi::TeamsListRoomsJob(this);
    job->setTeamId(teamId);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::TeamsListRoomsJob::teamListRoomsDone, this, &ChannelInfoEditableWidget::slotTeamListRoomsDone);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamsListRoomsJob job";
    }
}

void ChannelInfoEditableWidget::deleteTeam(const QByteArray &teamId, const QList<QByteArray> &roomIds)
{
    auto job = new RocketChatRestApi::TeamDeleteJob(this);
    job->setTeamId(teamId);
    job->setRoomsId(roomIds);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::TeamDeleteJob::deleteTeamDone, this, &ChannelInfoEditableWidget::slotTeamDeleteDone);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamsListRoomsJob job";
    }
}

void ChannelInfoEditableWidget::slotTeamListRoomsDone(const QJsonObject &obj)
{
    QList<TeamRoom> teamRooms = TeamRoom::parseTeamRooms(obj);
    const QByteArray teamId = mRoom->teamInfo().teamId();
    if (teamRooms.isEmpty()) {
        deleteTeam(teamId, {});
    } else {
        QPointer<TeamSelectDeletedRoomDialog> dlg = new TeamSelectDeletedRoomDialog(this);
        dlg->setTeamRooms(teamRooms);
        if (dlg->exec()) {
            const QList<QByteArray> roomIds = dlg->roomsId();
            auto job = new RocketChatRestApi::TeamDeleteJob(this);
            job->setRoomsId(roomIds);
            job->setTeamId(teamId);
            mRocketChatAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::TeamDeleteJob::deleteTeamDone, this, &ChannelInfoEditableWidget::slotTeamDeleteDone);
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamsListRoomsJob job";
            }
        }
        delete dlg;
    }
}

void ChannelInfoEditableWidget::slotTeamDeleteDone()
{
    Q_EMIT channelDeleted();
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
    if (mRoom->announcement() != mAnnouncement->toPlainText()) {
        info.roomAnnouncement = mAnnouncement->toPlainText();
        info.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::RoomAnnouncement;
    }
    if (mRoom->description() != mDescription->toPlainText()) {
        info.roomDescription = mDescription->toPlainText();
        info.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::RoomDescription;
    }
    if (mRoom->topic() != mComment->toPlainText()) {
        info.roomTopic = mComment->toPlainText();
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

    if (/*(mRoom->channelType() == Room::RoomType::Private) != mPrivate->isChecked()*/ 1) { // TODO verify
        info.roomType = mPrivate->isChecked() ? QStringLiteral("p") : QStringLiteral("c");
        info.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::RoomType;
    }

    if (hasRetentionPermission()) {
        mChannelInfoPruneWidget->saveRoomSettingsInfo(info, mRoom);
    }
    if (mRoomAvatarWidget->wasChanged()) {
        info.roomAvatar = mRoomAvatarWidget->roomAvatar();
        info.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::RoomAvatar;
    }
    // qDebug() << " info " << info;
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
    mPrivate->setChecked(mRoom->channelType() == Room::RoomType::Private);
    mEncrypted->setVisible(mRoom->encryptedEnabled());
    mEncrypted->setChecked(mRoom->encrypted());
    mEncryptedLabel->setVisible(mRoom->encryptedEnabled());
    mSystemMessageCombox->setMessagesSystem(mRoom->displaySystemMessageTypes());
    joinCodeChanged();
}

void ChannelInfoEditableWidget::joinCodeChanged()
{
    mPasswordLineEdit->lineEdit()->setPlaceholderText(mRoom->joinCodeRequired() ? i18nc("@info:placeholder", "This room has a password")
                                                                                : i18nc("@info:placeholder", "Add password"));
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
        mPrivate->setChecked(mRoom->channelType() == Room::RoomType::Private);
    });

    const Utils::AvatarInfo avatarInfo = mRoom->avatarInfo();
    const QString iconUrlStr = mRocketChatAccount->avatarUrl(avatarInfo);
    if (!iconUrlStr.isEmpty()) {
        const QString iconPath{QUrl(iconUrlStr).toLocalFile()};
        mRoomAvatarWidget->setCurrentIconPath(iconPath);
    }
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
    return mRoom->hasPermission(QStringLiteral("edit-room-retention-policy"));
}

#include "moc_channelinfoeditablewidget.cpp"
