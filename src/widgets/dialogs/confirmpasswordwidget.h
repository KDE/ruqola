/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>

class KPasswordLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfirmPasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfirmPasswordWidget(QWidget *parent = nullptr);
    ~ConfirmPasswordWidget() override;

    [[nodiscard]] QString password() const;

Q_SIGNALS:
    void enabledOkButton(bool enabled);

private:
    KPasswordLineEdit *const mPasswordLineEdit;
};
