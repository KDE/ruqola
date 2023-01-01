/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>

#include "libruqolawidgets_private_export.h"
class ConfirmPasswordWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfirmPasswordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfirmPasswordDialog(QWidget *parent = nullptr);
    ~ConfirmPasswordDialog() override;

    Q_REQUIRED_RESULT QString password() const;

private:
    ConfirmPasswordWidget *const mConfirmPasswordWidget;
};
