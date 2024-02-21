/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QWidget>

class PersonalAccessTokenAuthenticationConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PersonalAccessTokenAuthenticationConfigWidget(QWidget *parent = nullptr);
    ~PersonalAccessTokenAuthenticationConfigWidget() override;
};
