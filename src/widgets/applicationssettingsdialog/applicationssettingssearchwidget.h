/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "model/appsmarketplacefilterproxymodel.h"
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
    enum class Feature : uint8_t {
        None,
        Installed,
        Requested,
        Private,
    };
    Q_ENUM(Feature)

    explicit ApplicationsSettingsSearchWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsSearchWidget() override;

    [[nodiscard]] AppsMarketPlaceFilterProxyModel::FilterInfo filterInfo() const;
    [[nodiscard]] AppsMarketPlaceFilterProxyModel::Sorting sortingInfo() const;

    void setFeature(ApplicationsSettingsSearchWidget::Feature feature);

Q_SIGNALS:
    void filterChanged();
    void sortingChanged();

private:
    QLineEdit *const mSearchLineEdit;
    ApplicationsSettingsCategoriesComboBox *const mApplicationsSettingsCategoriesComboBox;
    ApplicationsSettingsPriceComboBox *const mApplicationsSettingsPriceComboBox;
    ApplicationsSettingsStatusComboBox *const mApplicationsSettingsStatusComboBox;
    ApplicationsSettingsSortingComboBox *const mApplicationsSettingsSortingComboBox;
};
