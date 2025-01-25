/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class RocketChatAccount;
class ApplicationsSettingsDescriptionTabWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsDescriptionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsDescriptionDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsDescriptionDialog() override;

    void setDescription(const QString &desc);

    void setApplicationId(const QByteArray &appId);

    void setShowLogAndSettingsInfo(bool state);

    void setShowVersionInfo(bool state);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    ApplicationsSettingsDescriptionTabWidget *const mApplicationsSettingsDescriptionTabWidget;
};
