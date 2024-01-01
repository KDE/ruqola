/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ParseMessageUrlUtilsTest : public QObject
{
    Q_OBJECT
public:
    explicit ParseMessageUrlUtilsTest(QObject *parent = nullptr);
    ~ParseMessageUrlUtilsTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldParseUrl();
    void shouldParseUrl_data();
};
