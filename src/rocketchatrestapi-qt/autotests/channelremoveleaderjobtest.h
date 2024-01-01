/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ChannelRemoveLeaderJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ChannelRemoveLeaderJobTest(QObject *parent = nullptr);
    ~ChannelRemoveLeaderJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
};
