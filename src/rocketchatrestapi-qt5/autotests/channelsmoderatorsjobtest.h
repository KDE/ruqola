/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ChannelsModeratorsJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ChannelsModeratorsJobTest(QObject *parent = nullptr);
    ~ChannelsModeratorsJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldNotStarting();
};

