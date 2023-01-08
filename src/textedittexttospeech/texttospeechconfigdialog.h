/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "textedittexttospeech_export.h"
#include <QDialog>
namespace TextEditTextToSpeech
{
class TextToSpeechConfigWidget;
class TEXTEDITTEXTTOSPEECH_EXPORT TextToSpeechConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TextToSpeechConfigDialog(QWidget *parent = nullptr);
    ~TextToSpeechConfigDialog() override;

private:
    void slotAccepted();
    void slotRestoreDefaults();
    void readConfig();
    void writeConfig();
    TextEditTextToSpeech::TextToSpeechConfigWidget *const mTextToSpeechConfigWidget;
};
}
