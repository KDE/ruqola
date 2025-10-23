/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
#include <QWidget>
namespace TextEditTextToSpeech
{
class TextToSpeechConfigWidget;
}
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureAccessibilityWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureAccessibilityWidget(QWidget *parent = nullptr);
    ~ConfigureAccessibilityWidget() override;

    void save();
    void load();
    void restoreToDefaults();

protected:
    void showEvent(QShowEvent *event) override;

private:
    TextEditTextToSpeech::TextToSpeechConfigWidget *const mTextToSpeechWidget;
    QCheckBox *const mEnableTextToSpeech;
    bool mWasInitialized = false;
};
