/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ResetPasswordWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ResetPasswordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ResetPasswordDialog(QWidget *parent = nullptr);
    ~ResetPasswordDialog() override;

    [[nodiscard]] QString email() const;

private:
    ResetPasswordWidget *const mResetPasswordWidget;
};
