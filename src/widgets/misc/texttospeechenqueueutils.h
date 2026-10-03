/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "texttospeech/texttospeechenqueueinfo.h"
#include <QString>
namespace TextEditTextToSpeech
{
class TextToSpeechContainerWidget;
}
namespace TextToSpeechEnqueueUtils
{
// Sends text to the speech synthesizer and stores info at the same time: the
// synthesizer identifies each utterance by its position in the queue, so both
// must always be done together. Pass a default constructed info when we don't
// have message information to update.
void enqueue(TextEditTextToSpeech::TextToSpeechContainerWidget *widget, const QString &text, const TextToSpeechEnqueueInfo &info = {});
}
