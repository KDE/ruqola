/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

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
    mainLayout->setObjectName(u"mainLayout"_s);

    mSearchLineEdit->setObjectName(u"mSearchLineEdit"_s);
    mainLayout->addWidget(mSearchLineEdit);
    KLineEditEventHandler::catchReturnKey(mSearchLineEdit);
    mSearchLineEdit->setClearButtonEnabled(true);

    mApplicationsSettingsCategoriesComboBox->setObjectName(u"mApplicationsSettingsCategoriesComboBox"_s);
    mainLayout->addWidget(mApplicationsSettingsCategoriesComboBox);

    mApplicationsSettingsPriceComboBox->setObjectName(u"mApplicationsSettingsPriceComboBox"_s);
    mainLayout->addWidget(mApplicationsSettingsPriceComboBox);

    mApplicationsSettingsStatusComboBox->setObjectName(u"mApplicationsSettingsStatusComboBox"_s);
    mainLayout->addWidget(mApplicationsSettingsStatusComboBox);

    mApplicationsSettingsSortingComboBox->setObjectName(u"mApplicationsSettingsSortingComboBox"_s);
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
