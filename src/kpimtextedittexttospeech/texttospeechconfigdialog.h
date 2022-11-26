/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedittexttospeech_export.h"
#include <QDialog>
namespace KPIMTextEditTextToSpeech
{
class TextToSpeechConfigWidget;
class KPIMTEXTEDITTEXTTOSPEECH_EXPORT TextToSpeechConfigDialog : public QDialog
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
    KPIMTextEditTextToSpeech::TextToSpeechConfigWidget *const mTextToSpeechConfigWidget;
};
}
