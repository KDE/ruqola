/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingssearchwidget.h"
#include "applicationssettingscategoriescombobox.h"
#include "applicationssettingspricecombobox.h"
#include "applicationssettingssortingcombobox.h"
#include "applicationssettingsstatuscombobox.h"
#include "rocketchataccount.h"

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLineEdit>
using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsSearchWidget::ApplicationsSettingsSearchWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mSearchLineEdit(new QLineEdit(this))
    , mApplicationsSettingsCategoriesComboBox(new ApplicationsSettingsCategoriesComboBox(account, this))
    , mApplicationsSettingsPriceComboBox(new ApplicationsSettingsPriceComboBox(this))
    , mApplicationsSettingsStatusComboBox(new ApplicationsSettingsStatusComboBox(this))
    , mApplicationsSettingsSortingComboBox(new ApplicationsSettingsSortingComboBox(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName("mainLayout"_L1);

    mSearchLineEdit->setObjectName("mSearchLineEdit"_L1);
    mainLayout->addWidget(mSearchLineEdit);
    KLineEditEventHandler::catchReturnKey(mSearchLineEdit);
    mSearchLineEdit->setClearButtonEnabled(true);

    mApplicationsSettingsCategoriesComboBox->setObjectName("mApplicationsSettingsCategoriesComboBox"_L1);
    mainLayout->addWidget(mApplicationsSettingsCategoriesComboBox);

    mApplicationsSettingsPriceComboBox->setObjectName("mApplicationsSettingsPriceComboBox"_L1);
    mainLayout->addWidget(mApplicationsSettingsPriceComboBox);

    mApplicationsSettingsStatusComboBox->setObjectName("mApplicationsSettingsStatusComboBox"_L1);
    mainLayout->addWidget(mApplicationsSettingsStatusComboBox);

    mApplicationsSettingsSortingComboBox->setObjectName("mApplicationsSettingsSortingComboBox"_L1);
    mainLayout->addWidget(mApplicationsSettingsSortingComboBox);

    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &ApplicationsSettingsSearchWidget::filterChanged);
    connect(mApplicationsSettingsCategoriesComboBox,
            &ApplicationsSettingsCategoriesComboBox::categoriesChanged,
            this,
            &ApplicationsSettingsSearchWidget::filterChanged);
    connect(mApplicationsSettingsPriceComboBox, &ApplicationsSettingsPriceComboBox::activated, this, &ApplicationsSettingsSearchWidget::filterChanged);
    connect(mApplicationsSettingsStatusComboBox, &ApplicationsSettingsStatusComboBox::activated, this, &ApplicationsSettingsSearchWidget::filterChanged);
    connect(mApplicationsSettingsSortingComboBox, &ApplicationsSettingsSortingComboBox::activated, this, &ApplicationsSettingsSearchWidget::sortingChanged);
}

ApplicationsSettingsSearchWidget::~ApplicationsSettingsSearchWidget() = default;

AppsMarketPlaceFilterProxyModel::FilterInfo ApplicationsSettingsSearchWidget::filterInfo() const
{
    AppsMarketPlaceFilterProxyModel::FilterInfo info;
    info.text = mSearchLineEdit->text();
    info.categories = mApplicationsSettingsCategoriesComboBox->categories();
    info.price = mApplicationsSettingsPriceComboBox->currentPrice();
    info.status = mApplicationsSettingsStatusComboBox->currentStatus();
    return info;
}

AppsMarketPlaceFilterProxyModel::Sorting ApplicationsSettingsSearchWidget::sortingInfo() const
{
    return mApplicationsSettingsSortingComboBox->currentSorting();
}

void ApplicationsSettingsSearchWidget::setFeature(Feature feature)
{
    switch (feature) {
    case Feature::None:
        mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search Applications"));
        break;
    case Feature::Installed:
        mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search Installed Applications"));
        break;
    case Feature::Requested:
        mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search Requested Applications"));
        break;
    case Feature::Private:
        mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search Private Applications"));
        break;
    }
}

#include "moc_applicationssettingssearchwidget.cpp"
