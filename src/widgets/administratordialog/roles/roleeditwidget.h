/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLineEdit;
class QCheckBox;
class RoleScopeComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RoleEditWidget : public QWidget
{
    Q_OBJECT
public:
    struct RoleEditDialogInfo {
        QString mName;
        QString mDescription;
        QString mScope;
        bool mTwoFactor = false;
        bool mIsProtected = false;
        Q_REQUIRED_RESULT bool isValid() const
        {
            return !mName.isEmpty();
        }
    };

    explicit RoleEditWidget(QWidget *parent = nullptr);
    ~RoleEditWidget() override;

    void setRoleEditDialogInfo(const RoleEditDialogInfo &info);
    Q_REQUIRED_RESULT RoleEditDialogInfo roleEditDialogInfo() const;

Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    QLineEdit *const mName;
    QLineEdit *const mDescription;
    QCheckBox *const mTwoFactor;
    RoleScopeComboBox *const mRoleScopeComboBox;
};
