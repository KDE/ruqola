/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "rooms/saveroomsettingsjob.h"
#include <QDialog>
class ChannelInfoWidget;
class Room;
class QDialogButtonBox;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ChannelInfoDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ChannelInfoDialog() override;
    void setRoom(Room *room);

    Q_REQUIRED_RESULT RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo saveRoomSettingsInfo() const;

private:
    void slotRoomNameValid(bool state);
    void slotFnameChanged(const QString &fname);
    ChannelInfoWidget *const mChannelInfoWidget;
    QDialogButtonBox *const mButtonBox;
    QPushButton *mOkButton = nullptr;
    bool mIsATeam = false;
};
