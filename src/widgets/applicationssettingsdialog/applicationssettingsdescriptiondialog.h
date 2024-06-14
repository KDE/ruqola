/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ApplicationsSettingsDescriptionWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsDescriptionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsDescriptionDialog(QWidget *parent = nullptr);
    ~ApplicationsSettingsDescriptionDialog() override;

    void setDescription(const QString &desc);

private:
    void readConfig();
    void writeConfig();
    ApplicationsSettingsDescriptionWidget *const mApplicationsSettingsDescriptionWidget;
};
