/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GetStarredMessagesJobTest : public QObject
{
    Q_OBJECT
public:
    explicit GetStarredMessagesJobTest(QObject *parent = nullptr);
    ~GetStarredMessagesJobTest() override = default;

private Q_SLOTS:
    void shouldGenerateRequest();
    void shouldNotStarting();
    void shouldHaveDefaultValue();
};
