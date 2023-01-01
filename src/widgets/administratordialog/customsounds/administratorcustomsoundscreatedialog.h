/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorcustomsoundscreatewidget.h"
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class AdministratorCustomSoundsCreateWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorCustomSoundsCreateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdministratorCustomSoundsCreateDialog(QWidget *parent = nullptr);
    ~AdministratorCustomSoundsCreateDialog() override;

    void setCustomSoundInfo(const AdministratorCustomSoundsCreateWidget::CustomSoundInfo &info);

    Q_REQUIRED_RESULT AdministratorCustomSoundsCreateWidget::CustomSoundInfo customSoundInfo() const;

private:
    void readConfig();
    void writeConfig();
    AdministratorCustomSoundsCreateWidget *const mCreateWidget;
};
