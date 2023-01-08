/*
  SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include <QObject>

class AutoCorrectionTextEditTest : public QObject
{
    Q_OBJECT
public:
    AutoCorrectionTextEditTest();
private Q_SLOTS:
    void shouldNotAutocorrectWhenDisabled();
    void shouldReplaceWordWhenExactText();
    void shouldNotReplaceWordWhenInexactText();
    void shouldReplaceWhenPressEnter();
    void shouldReplaceWhenPressReturn();
};
