/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QDialog>
#include <QList>

#include "directchannelinfowidget.h"
#include "libruqolawidgets_private_export.h"
class DirectChannelInfoWidget;
class User;
class RocketChatAccount;
class RoleInfo;
class Room;
class LIBRUQOLAWIDGETS_TESTS_EXPORT DirectChannelInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DirectChannelInfoDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~DirectChannelInfoDialog() override;

    void setDirectChannelInfo(const DirectChannelInfoWidget::DirectChannelInfo &info);

private:
    DirectChannelInfoWidget *const mDirectChannelInfoWidget;
};
