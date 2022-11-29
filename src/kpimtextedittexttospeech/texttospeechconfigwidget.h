/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "kpimtextedittexttospeech_export.h"
#include <QWidget>
class QSlider;
class QComboBox;
class QPushButton;
namespace KPIMTextEditTextToSpeech
{
class TextToSpeechLanguageComboBox;
class AbstractTextToSpeechConfigInterface;
class TextToSpeechSliderWidget;
class KPIMTEXTEDITTEXTTOSPEECH_EXPORT TextToSpeechConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextToSpeechConfigWidget(QWidget *parent = nullptr);
    ~TextToSpeechConfigWidget() override;

    void writeConfig();
    void readConfig();

    void setTextToSpeechConfigInterface(AbstractTextToSpeechConfigInterface *interface);
    void restoreDefaults();

Q_SIGNALS:
    void configChanged(bool state);

private:
    void slotTestTextToSpeech();
    void valueChanged();
    void slotLocalesAndVoices();
    void slotUpdateSettings();
    void slotEngineChanged();
    void slotLanguageChanged();
    void updateVoice();
    void updateLocale();
    void updateEngine();
    void updateAvailableLocales();
    void updateAvailableEngine();
    void updateAvailableVoices();
    TextToSpeechSliderWidget *const mVolume;
    TextToSpeechSliderWidget *const mRate;
    TextToSpeechSliderWidget *const mPitch;
    TextToSpeechLanguageComboBox *const mLanguage;
    AbstractTextToSpeechConfigInterface *mAbstractTextToSpeechConfigInterface = nullptr;
    QComboBox *const mAvailableEngine;
    QComboBox *const mVoice;
    QPushButton *const mTestButton;
};
}
