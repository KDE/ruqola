/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechenqueueutils.h"
#include "accountmanager.h"
#include "ruqola.h"
#include "texttospeech/texttospeechenqueuemanager.h"

#include <TextEditTextToSpeech/TextToSpeechContainerWidget>

void TextToSpeechEnqueueUtils::enqueue(TextEditTextToSpeech::TextToSpeechContainerWidget *widget, const QString &text, const TextToSpeechEnqueueInfo &info)
{
    // Insert first: when the synthesizer is idle it starts to speak (and reports
    // the utterance) from within enqueue().
    auto manager = Ruqola::self()->accountManager()->textToSpeechEnqueueManager();
    manager->insert(info);
    if (widget->enqueue(text) == -1) {
        // Nothing was enqueued (empty text or engine error), so no utterance will
        // be reported for it: don't shift the position of the next ones.
        manager->removeLast();
    }
}
