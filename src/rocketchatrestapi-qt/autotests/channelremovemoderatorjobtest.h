/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ChannelRemoveModeratorJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ChannelRemoveModeratorJobTest(QObject *parent = nullptr);
    ~ChannelRemoveModeratorJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
};
