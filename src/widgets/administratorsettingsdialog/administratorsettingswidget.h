/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>

class RocketChatAccount;
class QTabWidget;
class AccountSettingsWidget;
class EncryptionSettingsWidget;
class MessageSettingsWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorSettingsWidget() override;

    void initialize();

private:
    QTabWidget *const mTabWidget;
    AccountSettingsWidget *const mAccountSettingsWidget;
    EncryptionSettingsWidget *const mEncryptionSettingsWidget;
    MessageSettingsWidget *const mMessageSettingsWidget;
};
