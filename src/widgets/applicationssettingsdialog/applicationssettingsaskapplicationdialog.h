/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ApplicationsSettingsAskApplicationWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsAskApplicationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsAskApplicationDialog(QWidget *parent = nullptr);
    ~ApplicationsSettingsAskApplicationDialog() override;

    [[nodiscard]] QString message() const;

private:
    void readConfig();
    void writeConfig();
    ApplicationsSettingsAskApplicationWidget *const mApplicationsSettingsAskApplicationWidget;
};
