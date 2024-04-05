/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ConfigureActivitiesWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureActivitiesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigureActivitiesDialog(QWidget *parent = nullptr);
    ~ConfigureActivitiesDialog() override;

    [[nodiscard]] QStringList activities() const;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    ConfigureActivitiesWidget *const mConfigureActivitiesWidget;
};
