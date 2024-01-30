/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ResetPasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ResetPasswordWidget(QWidget *parent = nullptr);
    ~ResetPasswordWidget() override;

    [[nodiscard]] QString email() const;
Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotEmailChanged(const QString &str);
    QLineEdit *const mEmail;
};
