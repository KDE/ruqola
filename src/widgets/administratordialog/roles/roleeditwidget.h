/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDebug>
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
        [[nodiscard]] bool isValid() const
        {
            return !mName.isEmpty();
        }
        [[nodiscard]] bool operator==(const RoleEditDialogInfo &other) const;
        [[nodiscard]] bool operator!=(const RoleEditDialogInfo &other) const
        {
            return !operator==(other);
        }
    };

    explicit RoleEditWidget(QWidget *parent = nullptr);
    ~RoleEditWidget() override;

    void setRoleEditDialogInfo(const RoleEditDialogInfo &info);
    [[nodiscard]] RoleEditDialogInfo roleEditDialogInfo() const;

Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    QLineEdit *const mName;
    QLineEdit *const mDescription;
    QCheckBox *const mTwoFactor;
    RoleScopeComboBox *const mRoleScopeComboBox;
};
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const RoleEditWidget::RoleEditDialogInfo &t);
