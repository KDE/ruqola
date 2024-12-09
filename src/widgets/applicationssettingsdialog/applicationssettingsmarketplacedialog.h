/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "applicationssettingssearchwidget.h"
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ApplicationsSettingsWidgetBase;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsMarketPlaceDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsMarketPlaceDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsMarketPlaceDialog() override;

    void setFeature(ApplicationsSettingsSearchWidget::Feature feature);

    void initialize();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    ApplicationsSettingsWidgetBase *const mApplicationsSettingsWidget;
};
