/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "textedittexttospeech_private_export.h"
#include <QWidget>
class QLabel;
class QSlider;
namespace TextEditTextToSpeech
{
class TEXTEDITTEXTTOSPEECH_TESTS_EXPORT TextToSpeechSliderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextToSpeechSliderWidget(const QString &labelInfo, QWidget *parent = nullptr);
    ~TextToSpeechSliderWidget() override;

    void setValue(int value);
    void setRange(int min, int max);
    Q_REQUIRED_RESULT int value() const;

Q_SIGNALS:
    void valueChanged(int value);

private:
    void slotValueChanged(int value);
    QString mLabelInfo;
    QLabel *const mLabel;
    QSlider *const mSlider;
};
}
