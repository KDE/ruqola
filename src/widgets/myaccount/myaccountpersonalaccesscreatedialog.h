/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class MyAccountPersonalAccessCreateWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccountPersonalAccessCreateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MyAccountPersonalAccessCreateDialog(QWidget *parent = nullptr);
    ~MyAccountPersonalAccessCreateDialog() override;

    [[nodiscard]] bool bypassTwoFactor() const;
    [[nodiscard]] QString tokenName() const;

    void setExistingTokenNames(const QStringList &lst);

private:
    MyAccountPersonalAccessCreateWidget *const mCreateWidget;
};
