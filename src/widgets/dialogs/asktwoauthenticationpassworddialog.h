/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class QPushButton;
class AskTwoAuthenticationPasswordWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AskTwoAuthenticationPasswordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AskTwoAuthenticationPasswordDialog(QWidget *parent = nullptr);
    ~AskTwoAuthenticationPasswordDialog() override;
    Q_REQUIRED_RESULT QString code() const;

private:
    AskTwoAuthenticationPasswordWidget *const mAskTwoAuthicationPasswordWidget;
    QPushButton *mOkButton = nullptr;
};

