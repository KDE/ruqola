/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TextToSpeechSliderWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit TextToSpeechSliderWidgetTest(QObject *parent = nullptr);
    ~TextToSpeechSliderWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
