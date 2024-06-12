/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingscategoriescombobox.h"
#include "rocketchataccount.h"

ApplicationsSettingsCategoriesComboBox::ApplicationsSettingsCategoriesComboBox(RocketChatAccount *account, QWidget *parent)
    : QComboBox(parent)
{
    // Add category model
}

ApplicationsSettingsCategoriesComboBox::~ApplicationsSettingsCategoriesComboBox() = default;

#include "moc_applicationssettingscategoriescombobox.cpp"
