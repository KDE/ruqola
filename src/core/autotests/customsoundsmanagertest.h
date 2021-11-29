/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class CustomSoundsManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit CustomSoundsManagerTest(QObject *parent = nullptr);
    ~CustomSoundsManagerTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();

    void shouldParseCustomSounds();
    void shouldParseCustomSounds_data();

    void shouldDeleteCustomSounds();
    void shouldDeleteCustomSounds_data();

    void shouldAddCustomSounds();
    void shouldAddCustomSounds_data();

    void shouldUpdateCustomSounds();
    void shouldUpdateCustomSounds_data();
};
