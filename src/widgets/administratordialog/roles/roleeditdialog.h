/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include "roleeditwidget.h"
#include <QDialog>
class LIBRUQOLAWIDGETS_TESTS_EXPORT RoleEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RoleEditDialog(QWidget *parent = nullptr);
    ~RoleEditDialog() override;

    void setRoleEditDialogInfo(const RoleEditWidget::RoleEditDialogInfo &info);
    Q_REQUIRED_RESULT RoleEditWidget::RoleEditDialogInfo roleEditDialogInfo() const;

private:
    RoleEditWidget *const mRoleEditWidget;
};
