/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UserTest : public QObject
{
    Q_OBJECT
public:
    explicit UserTest(QObject *parent = nullptr);
    ~UserTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldSetAndGetName();
    void shouldSetAndGetStatus();
    void shouldSetAndGetUserId();
    void shouldParseUser();
    void checkEqualsAndUnequalsOperator();

    void shouldParseJson_data();
    void shouldParseJson();

    void shouldGetStatusIcon_data();
    void shouldGetStatusIcon();

    void shouldBeValid();

    void shouldParseRestApiJson_data();
    void shouldParseRestApiJson();
};
