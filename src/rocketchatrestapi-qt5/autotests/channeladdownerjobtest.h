/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ChannelAddOwnerJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ChannelAddOwnerJobTest(QObject *parent = nullptr);
    ~ChannelAddOwnerJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
};
