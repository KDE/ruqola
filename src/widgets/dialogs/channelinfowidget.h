/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

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
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelInfoWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ChannelInfoWidget() override;

    void setRoom(Room *room);

    void updateUiFromPermission();

    Q_REQUIRED_RESULT RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo saveRoomSettingsInfo() const;
Q_SIGNALS:
    void channelDeleted();
    void fnameChanged(const QString &fname);
    void roomNameValid(bool valid);

private:
    QStackedWidget *const mStackedWidget;
    Room *mRoom = nullptr;
    ChannelInfoEditableWidget *const mChannelInfoEditableWidget;
    ChannelInfoReadOnlyWidget *const mChannelInfoReadOnlyWidget;
};

