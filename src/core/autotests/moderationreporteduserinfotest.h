/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ModerationReportedUserInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit ModerationReportedUserInfoTest(QObject *parent = nullptr);
    ~ModerationReportedUserInfoTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
