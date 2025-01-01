/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "createchannelteaminfo.h"
#include "createnewchannelwidget.h"
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class QPushButton;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewChannelDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateNewChannelDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~CreateNewChannelDialog() override;
    [[nodiscard]] RocketChatRestApi::CreateChannelTeamInfo channelInfo(bool userMemberUserId) const;

    void setFeatures(CreateNewChannelWidget::Features features);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    CreateNewChannelWidget *const mCreateNewChannelWidget;
};
