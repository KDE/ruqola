/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingssearchwidget.h"
#include "applicationssettingscombobox.h"
#include "applicationssettingspricecombobox.h"
#include <QHBoxLayout>
#include <QLineEdit>
using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsSearchWidget::ApplicationsSettingsSearchWidget(QWidget *parent)
    : QWidget{parent}
    , mSearchLineEdit(new QLineEdit(this))
    , mApplicationsSettingsComboBox(new ApplicationsSettingsComboBox(this))
    , mApplicationsSettingsPriceComboBox(new ApplicationsSettingsPriceComboBox(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName({});

    mApplicationsSettingsComboBox->setObjectName("mApplicationsSettingsComboBox"_L1);
    mainLayout->addWidget(mApplicationsSettingsComboBox);

    mApplicationsSettingsPriceComboBox->setObjectName("mApplicationsSettingsPriceComboBox"_L1);
    mainLayout->addWidget(mApplicationsSettingsPriceComboBox);
    // TODO
}

ApplicationsSettingsSearchWidget::~ApplicationsSettingsSearchWidget() = default;
