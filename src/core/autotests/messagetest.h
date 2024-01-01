/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MessageTest : public QObject
{
    Q_OBJECT
public:
    explicit MessageTest(QObject *parent = nullptr);
    ~MessageTest() override = default;
private Q_SLOTS:

    void shouldHaveDefaultValues();

    void shouldParseMessage_data();
    void shouldParseMessage();
    void shouldSerializeData();

    void shouldParseJsonMessage_data();
    void shouldParseJsonMessage();

    void shouldUpdateJsonMessage_data();
    void shouldUpdateJsonMessage();
};
