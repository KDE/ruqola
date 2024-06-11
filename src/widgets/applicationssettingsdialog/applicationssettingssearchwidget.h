/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class ApplicationsSettingsComboBox;
class ApplicationsSettingsPriceComboBox;
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsSearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsSearchWidget(QWidget *parent = nullptr);
    ~ApplicationsSettingsSearchWidget() override;

private:
    QLineEdit *const mSearchLineEdit;
    ApplicationsSettingsComboBox *const mApplicationsSettingsComboBox;
    ApplicationsSettingsPriceComboBox *const mApplicationsSettingsPriceComboBox;
};
