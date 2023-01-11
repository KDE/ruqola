/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QMainWindow>
class QTextEdit;
namespace TextEditTextToSpeech
{
class TextToSpeechContainerWidget;
}
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
    TextEditTextToSpeech::TextToSpeechContainerWidget *mContainerWidget = nullptr;
};
