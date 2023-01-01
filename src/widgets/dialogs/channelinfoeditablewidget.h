/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "rooms/saveroomsettingsjob.h"

#include <QWidget>
class MessageTextEditor;
class QCheckBox;
class QPushButton;
class Room;
class ChannelInfoPruneWidget;
class KPasswordLineEdit;
class SystemMessagesComboBox;
class QLineEdit;
class RoomAvatarWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelInfoEditableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelInfoEditableWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ChannelInfoEditableWidget() override;
    void setRoom(Room *room);
    Q_REQUIRED_RESULT RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo saveRoomSettingsInfo() const;

    void updateEditableChannelInfo();
    void connectEditableWidget();
    void updateRetentionValue();
    void updateUiFromPermission();
Q_SIGNALS:
    void channelDeleted();
    void fnameChanged(const QString &fname);
    void roomNameValid(bool valid);

private:
    void joinCodeChanged();
    void slotTeamDeleteDone();
    void slotTeamListRoomsDone(const QJsonObject &obj);
    void selectRoomToDelete(const QString &teamId);
    void deleteTeam(const QString &teamId, const QStringList &roomdId);
    Q_REQUIRED_RESULT bool hasRetentionPermission() const;
    QPointer<Room> mRoom;

    QLineEdit *const mName;
    MessageTextEditor *const mComment;
    MessageTextEditor *const mAnnouncement;
    MessageTextEditor *const mDescription;
    KPasswordLineEdit *const mPasswordLineEdit;
    QCheckBox *const mReadOnly;
    QCheckBox *const mArchive;
    QCheckBox *const mPrivate;
    QCheckBox *const mEncrypted;
    QPushButton *const mDeleteChannel;
    QWidget *mEncryptedLabel = nullptr;
    ChannelInfoPruneWidget *const mChannelInfoPruneWidget;
    SystemMessagesComboBox *const mSystemMessageCombox;
    RoomAvatarWidget *const mRoomAvatarWidget;
    RocketChatAccount *const mRocketChatAccount;
};
