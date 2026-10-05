/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
namespace TextSpeechToText
{
class SpeechToTextConfigureWidget;
}
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureSpeechToTextConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureSpeechToTextConfigureWidget(QWidget *parent = nullptr);
    ~ConfigureSpeechToTextConfigureWidget() override;

    void save();
    void load();
    void restoreToDefaults();

private:
    TextSpeechToText::SpeechToTextConfigureWidget *const mSpeechToTextWidget;
    QCheckBox *const mEnableSpeechToText;
};
