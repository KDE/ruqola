/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingscategoriescombobox.h"
#include "model/appscategoriesmodel.h"
#include "rocketchataccount.h"

ApplicationsSettingsCategoriesComboBox::ApplicationsSettingsCategoriesComboBox(RocketChatAccount *account, QWidget *parent)
    : QComboBox(parent)
    , mRocketChatAccount(account)
{
    if (account) {
        setModel(account->appsCategoriesModel());
        connect(account->appsCategoriesModel(), &AppsCategoriesModel::itemChanged, this, &ApplicationsSettingsCategoriesComboBox::categoriesChanged);
    }
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
}

ApplicationsSettingsCategoriesComboBox::~ApplicationsSettingsCategoriesComboBox() = default;

QStringList ApplicationsSettingsCategoriesComboBox::categories() const
{
    return mRocketChatAccount->appsCategoriesModel()->categoriesSelected();
}

#include "moc_applicationssettingscategoriescombobox.cpp"
