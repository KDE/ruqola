/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class RuqolaCommandLineParserTest : public QObject
{
    Q_OBJECT
public:
    explicit RuqolaCommandLineParserTest(QObject *parent = nullptr);
    ~RuqolaCommandLineParserTest() override = default;
private Q_SLOTS:
    void shouldVerifyCommandLineFromEnumValues();
};
