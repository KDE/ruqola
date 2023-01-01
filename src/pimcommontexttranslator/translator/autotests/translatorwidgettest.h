/*
  SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include <QObject>

class TranslatorWidgetTest : public QObject
{
    Q_OBJECT
public:
    TranslatorWidgetTest();

private Q_SLOTS:
    void shouldHaveDefaultValuesOnCreation();
    void shouldEnableTranslateButtonWhenTextToTranslateIsNotEmpty();
    void shouldDisableTranslateButtonAndClearTextWhenClickOnClearButton();
    void shouldInvertLanguageWhenClickOnInvertButton();
    void shouldHideWidgetWhenPressEscape();
    void shouldEmitTranslatorWasClosedSignalWhenCloseIt();
};
