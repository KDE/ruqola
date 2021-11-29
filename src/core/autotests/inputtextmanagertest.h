/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class InputTextManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit InputTextManagerTest(QObject *parent = nullptr);
    ~InputTextManagerTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValue();

    void shouldReplaceWord_data();
    void shouldReplaceWord();

    void shouldSearchWord_data();
    void shouldSearchWord();

    void shouldEmitCompletionRequestSignals();
};

