/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class PersonalAccessTokenAuthenticationConfigDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit PersonalAccessTokenAuthenticationConfigDialogTest(QObject *parent = nullptr);
    ~PersonalAccessTokenAuthenticationConfigDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldEnableOkButton();
};
