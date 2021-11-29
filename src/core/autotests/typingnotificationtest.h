/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TypingNotificationTest : public QObject
{
    Q_OBJECT
public:
    explicit TypingNotificationTest(QObject *parent = nullptr);
    ~TypingNotificationTest() override = default;
private Q_SLOTS:
    void shouldNotEmitSignalByDefault();
    void shouldEmitSignalWhenTyping();
    void shouldEmitSignalWhenTypingAndEmitTypingFalseAfterTimeout();
    void shouldDontEmitSignalWhenTypingSeveralTextBeforeTimeOut();
    void shouldEmitTwoSignalWhenChangeRoom();
};

