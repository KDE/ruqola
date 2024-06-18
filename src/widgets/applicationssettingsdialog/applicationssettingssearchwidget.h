/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "model/appsmarketplacefilterproxymodel.h"
#include "rocketchataccount.h"
#include <QWidget>
class ApplicationsSettingsCategoriesComboBox;
class ApplicationsSettingsPriceComboBox;
class ApplicationsSettingsStatusComboBox;
class QLineEdit;
class RocketChatAccount;
class ApplicationsSettingsSortingComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsSearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsSearchWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsSearchWidget() override;

    [[nodiscard]] AppsMarketPlaceFilterProxyModel::FilterInfo filterInfo() const;
Q_SIGNALS:
    void filterChanged();

private:
    QLineEdit *const mSearchLineEdit;
    ApplicationsSettingsCategoriesComboBox *const mApplicationsSettingsComboBox;
    ApplicationsSettingsPriceComboBox *const mApplicationsSettingsPriceComboBox;
    ApplicationsSettingsStatusComboBox *const mApplicationsSettingsStatusComboBox;
    ApplicationsSettingsSortingComboBox *const mApplicationsSettingsSortingComboBox;
};
