/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "applicationssettingssearchwidget.h"
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ApplicationsSettingsWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsDialog() override;

    void setFeature(ApplicationsSettingsSearchWidget::Feature feature);

    void initialize();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    ApplicationsSettingsWidget *const mApplicationsSettingsWidget;
};
