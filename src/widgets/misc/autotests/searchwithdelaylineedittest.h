/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SearchWithDelayLineEditTest : public QObject
{
    Q_OBJECT
public:
    explicit SearchWithDelayLineEditTest(QObject *parent = nullptr);
    ~SearchWithDelayLineEditTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

