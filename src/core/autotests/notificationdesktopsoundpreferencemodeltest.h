/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class NotificationDesktopSoundPreferenceModelTest : public QObject
{
    Q_OBJECT
public:
    explicit NotificationDesktopSoundPreferenceModelTest(QObject *parent = nullptr);
    ~NotificationDesktopSoundPreferenceModelTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};
