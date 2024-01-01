/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TranslateTextJobTest : public QObject
{
    Q_OBJECT
public:
    explicit TranslateTextJobTest(QObject *parent = nullptr);
    ~TranslateTextJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
