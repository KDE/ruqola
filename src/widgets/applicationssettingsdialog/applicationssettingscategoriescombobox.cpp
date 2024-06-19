/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingscategoriescombobox.h"
#include "model/appscategoriesmodel.h"
#include "rocketchataccount.h"

ApplicationsSettingsCategoriesComboBox::ApplicationsSettingsCategoriesComboBox(RocketChatAccount *account, QWidget *parent)
    : QComboBox(parent)
    , mRocketChatAccount(account)
{
    // Add category model
    if (account) {
        setModel(account->appsCategoriesModel());
    }
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
}

ApplicationsSettingsCategoriesComboBox::~ApplicationsSettingsCategoriesComboBox() = default;

QStringList ApplicationsSettingsCategoriesComboBox::categories() const
{
    return mRocketChatAccount->appsCategoriesModel()->categoriesSelected();
}

#include "moc_applicationssettingscategoriescombobox.cpp"
