/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "rocketchataccount.h"
#include <QTreeView>
class ApplicationsSettingsDelegate;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsListView : public QTreeView
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsListView(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsListView() override;

private:
    ApplicationsSettingsDelegate *const mApplicationsSettingsListDelegate;
};
