/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class TextToSpeechConfigGui : public QWidget
{
    Q_OBJECT
public:
    explicit TextToSpeechConfigGui(QWidget *parent = nullptr);
    ~TextToSpeechConfigGui() override;
};
