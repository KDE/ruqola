/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QDialog>
#include <QVector>

#include "libruqolawidgets_private_export.h"
class DirectChannelInfoWidget;
class User;
class RocketChatAccount;
class RoleInfo;
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
