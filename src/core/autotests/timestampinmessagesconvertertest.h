/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class TimeStampInMessagesConverterTest : public QObject
{
    Q_OBJECT
public:
    explicit TimeStampInMessagesConverterTest(QObject *parent = nullptr);
    ~TimeStampInMessagesConverterTest() override = default;

private Q_SLOTS:
    void shouldConvertTimeStampInMessages_data();
    void shouldConvertTimeStampInMessages();

    void shouldConvertTimeStamp_data();
    void shouldConvertTimeStamp();

    void shouldRegularExpressionStrValue();

    void shouldCalculateRelativeTime_data();
    void shouldCalculateRelativeTime();
};
