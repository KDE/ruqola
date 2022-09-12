/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccountPersonalAccessCreateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccountPersonalAccessCreateWidget(QWidget *parent = nullptr);
    ~MyAccountPersonalAccessCreateWidget() override;

    Q_REQUIRED_RESULT bool bypassTwoFactor() const;
    Q_REQUIRED_RESULT QString tokenName() const;
Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    QCheckBox *const mBypassTwoFactor;
    QLineEdit *const mTokenName;
};
