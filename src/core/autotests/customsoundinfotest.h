/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class CustomSoundInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit CustomSoundInfoTest(QObject *parent = nullptr);
    ~CustomSoundInfoTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
