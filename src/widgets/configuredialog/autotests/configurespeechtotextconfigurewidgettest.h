/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ConfigureSpeechToTextConfigureWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ConfigureSpeechToTextConfigureWidgetTest(QObject *parent = nullptr);
    ~ConfigureSpeechToTextConfigureWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
