/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TeamConvertToChannelJobTest : public QObject
{
    Q_OBJECT
public:
    explicit TeamConvertToChannelJobTest(QObject *parent = nullptr);
    ~TeamConvertToChannelJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
    void shouldNotStarting();
};
