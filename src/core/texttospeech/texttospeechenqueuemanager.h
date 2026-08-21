/*
  SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include "libruqolacore_export.h"
#include "texttospeechenqueueinfo.h"
#include <QList>
#include <QObject>

// Stores the message information of each text sent to the speech synthesizer.
// TextEditTextToSpeech::TextToSpeech identifies an utterance by its position in
// the queue, so we must insert exactly one element per enqueued text, in the
// same order. Use TextToSpeechEnqueueUtils::enqueue() which does both, and
// insert a default constructed (invalid) info when we don't have information
// about the message.
class LIBRUQOLACORE_EXPORT TextToSpeechEnqueueManager : public QObject
{
    Q_OBJECT
public:
    explicit TextToSpeechEnqueueManager(QObject *parent = nullptr);
    ~TextToSpeechEnqueueManager() override;
    void clear();

    [[nodiscard]] bool contains(const TextToSpeechEnqueueInfo &info) const;

    void insert(const TextToSpeechEnqueueInfo &info);

    // Removes the last inserted element, when the text was not enqueued after all.
    void removeLast();

    [[nodiscard]] TextToSpeechEnqueueInfo value(qsizetype index) const;

    [[nodiscard]] QList<TextToSpeechEnqueueInfo> enqueueList() const;

private:
    QList<TextToSpeechEnqueueInfo> mEnqueueList;
};
