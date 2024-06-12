/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QComboBox>

class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsComboBox(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsComboBox() override;
};
