/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] bool bypassTwoFactor() const;
    [[nodiscard]] QString tokenName() const;
    void setExistingTokenNames(const QStringList &lst);
Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    QStringList mExistingTokenNames;
    QCheckBox *const mBypassTwoFactor;
    QLineEdit *const mTokenName;
};
