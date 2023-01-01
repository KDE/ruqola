/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "createchannelteaminfo.h"
#include "createnewchannelwidget.h"
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class CreateNewChannelWidget;
class QPushButton;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewChannelDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateNewChannelDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~CreateNewChannelDialog() override;
    Q_REQUIRED_RESULT RocketChatRestApi::CreateChannelTeamInfo channelInfo(bool userMemberUserId) const;

    void setFeatures(CreateNewChannelWidget::Features features);

private:
    void writeConfig();
    void readConfig();
    CreateNewChannelWidget *const mCreateNewChannelWidget;
    QPushButton *mOkButton = nullptr;
};
