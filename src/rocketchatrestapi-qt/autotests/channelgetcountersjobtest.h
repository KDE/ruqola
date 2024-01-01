/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ChannelGetCountersJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ChannelGetCountersJobTest(QObject *parent = nullptr);
    ~ChannelGetCountersJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldNotStarting();
};
