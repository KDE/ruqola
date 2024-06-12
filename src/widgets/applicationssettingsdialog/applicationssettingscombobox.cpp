/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingscombobox.h"
#include "rocketchataccount.h"

ApplicationsSettingsComboBox::ApplicationsSettingsComboBox(RocketChatAccount *account, QWidget *parent)
    : QComboBox(parent)
{
    // Add category model
}

ApplicationsSettingsComboBox::~ApplicationsSettingsComboBox() = default;

#include "moc_applicationssettingscombobox.cpp"
