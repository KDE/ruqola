/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>
class PersonalAccessTokenAuthenticationConfigWidget;
class PersonalAccessTokenAuthenticationConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PersonalAccessTokenAuthenticationConfigDialog(QWidget *parent = nullptr);
    ~PersonalAccessTokenAuthenticationConfigDialog() override;

private:
    PersonalAccessTokenAuthenticationConfigWidget *const mPersonalAccessTokenAuthenticationConfigWidget;
};
