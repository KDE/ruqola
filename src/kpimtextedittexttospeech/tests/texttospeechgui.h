/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QMainWindow>
class QTextEdit;
class TextToSpeechGui : public QMainWindow
{
    Q_OBJECT
public:
    explicit TextToSpeechGui(QWidget *parent = nullptr);
    ~TextToSpeechGui() override;

private Q_SLOTS:
    void slotTextToSpeech();

private:
    QTextEdit *mEdit = nullptr;
};
