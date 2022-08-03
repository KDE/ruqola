/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class FindOrCreateInviteJobTest : public QObject
{
    Q_OBJECT
public:
    explicit FindOrCreateInviteJobTest(QObject *parent = nullptr);
    ~FindOrCreateInviteJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
    void shouldNotStarting();
    void shouldParseResult();
    void shouldParseResult_data();
};
