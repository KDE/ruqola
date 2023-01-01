/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ChannelTest : public QObject
{
    Q_OBJECT
public:
    explicit ChannelTest(QObject *parent = nullptr);
    ~ChannelTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();

    void shouldAssignValue();
    void shouldCopyValue();

    void shouldParseChannel_data();
    void shouldParseChannel();
};
