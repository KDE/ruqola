/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

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
    explicit ChannelInfoDialog(Room *room, RocketChatAccount *account, QWidget *parent = nullptr);
    ~ChannelInfoDialog() override;

    [[nodiscard]] RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo saveRoomSettingsInfo() const;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRoomNameValid(bool state);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFnameChanged(const QString &fname);
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    ChannelInfoWidget *const mChannelInfoWidget;
    QDialogButtonBox *const mButtonBox;
    QPushButton *mOkButton = nullptr;
    bool mIsATeam = false;
};
