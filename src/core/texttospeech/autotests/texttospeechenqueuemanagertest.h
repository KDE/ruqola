/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QObject>

class TextToSpeechEnqueueManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit TextToSpeechEnqueueManagerTest(QObject *parent = nullptr);
    ~TextToSpeechEnqueueManagerTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldStoreInfoAtEnqueuePosition();
    void shouldNotContainsInvalidInfo();
    void shouldClearList();
    void shouldRemoveLastInfo();
};
