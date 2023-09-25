/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ModerationInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit ModerationInfoTest(QObject *parent = nullptr);
    ~ModerationInfoTest() override = default;

private Q_SLOTS:
    void shouldModerationInfo_data();
    void shouldModerationInfo();
    void shouldHaveDefaultValues();
};
