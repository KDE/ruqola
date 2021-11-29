/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
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

