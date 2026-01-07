/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class TimeStampInMessagesWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit TimeStampInMessagesWidgetTest(QObject *parent = nullptr);
    ~TimeStampInMessagesWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldHaveTimeStampInfoDefaultValues();
};
