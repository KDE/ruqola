/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class UnbanUserInChannelJobTest : public QObject
{
    Q_OBJECT
public:
    explicit UnbanUserInChannelJobTest(QObject *parent = nullptr);
    ~UnbanUserInChannelJobTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
