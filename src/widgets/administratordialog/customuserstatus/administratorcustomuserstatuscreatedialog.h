/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorcustomuserstatuscreatewidget.h"
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorCustomUserStatusCreateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdministratorCustomUserStatusCreateDialog(QWidget *parent = nullptr);
    ~AdministratorCustomUserStatusCreateDialog() override;

    [[nodiscard]] AdministratorCustomUserStatusCreateWidget::UserStatusInfo userStatusInfo() const;
    void setUserStatusInfo(const AdministratorCustomUserStatusCreateWidget::UserStatusInfo &info);

private:
    void readConfig();
    void writeConfig();
    AdministratorCustomUserStatusCreateWidget *const mCreateWidget;
    QPushButton *mOkButton = nullptr;
};
