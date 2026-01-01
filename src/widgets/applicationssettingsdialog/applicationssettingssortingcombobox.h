/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "model/appsmarketplacefilterproxymodel.h"
#include <QComboBox>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsSortingComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsSortingComboBox(QWidget *parent = nullptr);
    ~ApplicationsSettingsSortingComboBox() override;

    [[nodiscard]] AppsMarketPlaceFilterProxyModel::Sorting currentSorting() const;
};
