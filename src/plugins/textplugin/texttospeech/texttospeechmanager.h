/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
class QTextToSpeech;
class TextToSpeechManager : public QObject
{
    Q_OBJECT
public:
    explicit TextToSpeechManager(QObject *parent = nullptr);
    ~TextToSpeechManager() override;

    static TextToSpeechManager *self();

    Q_REQUIRED_RESULT QTextToSpeech *textToSpeech();

private:
    QTextToSpeech *mTextToSpeech = nullptr;
};
