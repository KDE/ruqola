/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
