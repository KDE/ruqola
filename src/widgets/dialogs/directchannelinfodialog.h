/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "roles/roleinfo.h"
#include <QDialog>
#include <QVector>

#include "libruqolawidgets_private_export.h"
class DirectChannelInfoWidget;
class User;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT DirectChannelInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DirectChannelInfoDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~DirectChannelInfoDialog() override;
    void setUserName(const QString &userName);
    void setRoles(const QVector<RoleInfo> &newRoles);

private:
    void readConfig();
    void writeConfig();
    DirectChannelInfoWidget *const mDirectChannelInfoWidget;
};

