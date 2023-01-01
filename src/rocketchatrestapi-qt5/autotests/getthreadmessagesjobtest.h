/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GetThreadMessagesJobTest : public QObject
{
    Q_OBJECT
public:
    explicit GetThreadMessagesJobTest(QObject *parent = nullptr);
    ~GetThreadMessagesJobTest() override = default;

private Q_SLOTS:
    void shouldGenerateRequest();
    void shouldNotStarting();
    void shouldHaveDefaultValue();
};
