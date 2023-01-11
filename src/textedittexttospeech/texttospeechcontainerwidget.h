/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "textedittexttospeech_export.h"
#include <QWidget>
namespace TextEditTextToSpeech
{
class TextToSpeechContainerWidgetPrivate;
class TEXTEDITTEXTTOSPEECH_EXPORT TextToSpeechContainerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextToSpeechContainerWidget(QWidget *parent = nullptr);
    ~TextToSpeechContainerWidget() override;

    void say(const QString &text);

private:
    void slotChangeVisibility(bool state);
    std::unique_ptr<TextToSpeechContainerWidgetPrivate> const d;
};
}
