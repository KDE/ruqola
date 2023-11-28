/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "rooms/saveroomsettingsjob.h"

#include <QWidget>
class QStackedWidget;
class Room;
class ChannelInfoReadOnlyWidget;
class ChannelInfoEditableWidget;
class RocketChatAccount;
class ChannelRolesInfoWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelInfoWidget(Room *room, RocketChatAccount *account, QWidget *parent = nullptr);
    ~ChannelInfoWidget() override;

    void updateUiFromPermission();

    [[nodiscard]] RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo saveRoomSettingsInfo() const;
Q_SIGNALS:
    void channelDeleted();
    void fnameChanged(const QString &fname);
    void roomNameValid(bool valid);

private:
    void setRoom(Room *room);
    QStackedWidget *const mStackedWidget;
    QPointer<Room> mRoom;
    ChannelInfoEditableWidget *const mChannelInfoEditableWidget;
    ChannelInfoReadOnlyWidget *const mChannelInfoReadOnlyWidget;
    ChannelRolesInfoWidget *const mChannelRolesInfoWidget;
};
