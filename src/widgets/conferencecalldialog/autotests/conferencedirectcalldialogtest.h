/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ConferenceDirectCallDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit ConferenceDirectCallDialogTest(QObject *parent = nullptr);
    ~ConferenceDirectCallDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
