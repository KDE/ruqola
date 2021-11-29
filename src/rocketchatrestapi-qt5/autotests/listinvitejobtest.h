/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ListInviteJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ListInviteJobTest(QObject *parent = nullptr);
    ~ListInviteJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldNotStarting();
};
