/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class FetchUsersWaitingForGroupKeyJobTest : public QObject
{
    Q_OBJECT
public:
    explicit FetchUsersWaitingForGroupKeyJobTest(QObject *parent = nullptr);
    ~FetchUsersWaitingForGroupKeyJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
