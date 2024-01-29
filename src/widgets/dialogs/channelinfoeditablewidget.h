/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

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
    explicit ChannelInfoEditableWidget(Room *room, RocketChatAccount *account, QWidget *parent = nullptr);
    ~ChannelInfoEditableWidget() override;
    [[nodiscard]] RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo saveRoomSettingsInfo() const;

    void updateEditableChannelInfo();
    void connectEditableWidget();
    void updateRetentionValue();
    void updateUiFromPermission();
Q_SIGNALS:
    void channelDeleted();
    void fnameChanged(const QString &fname);
    void roomNameValid(bool valid);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void joinCodeChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTeamDeleteDone();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTeamListRoomsDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void selectRoomToDelete(const QString &teamId);
    LIBRUQOLAWIDGETS_NO_EXPORT void deleteTeam(const QString &teamId, const QStringList &roomdId);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT bool hasRetentionPermission() const;
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
