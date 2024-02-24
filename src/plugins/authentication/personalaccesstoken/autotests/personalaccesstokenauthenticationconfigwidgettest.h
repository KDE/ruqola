/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class PersonalAccessTokenAuthenticationConfigWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit PersonalAccessTokenAuthenticationConfigWidgetTest(QObject *parent = nullptr);
    ~PersonalAccessTokenAuthenticationConfigWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldEmitSignalEnableOkButton();
};
