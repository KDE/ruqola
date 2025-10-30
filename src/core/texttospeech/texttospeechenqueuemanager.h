/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include "libruqolacore_export.h"
#include "texttospeechenqueueinfo.h"
#include <QObject>

class LIBRUQOLACORE_EXPORT TextToSpeechEnqueueManager : public QObject
{
    Q_OBJECT
public:
    explicit TextToSpeechEnqueueManager(QObject *parent = nullptr);
    ~TextToSpeechEnqueueManager() override;
    void clear();

    [[nodiscard]] TextToSpeechEnqueueInfo textToSpeechInfo(qsizetype index);
    [[nodiscard]] bool contains(qsizetype index) const;
    void insert(qsizetype index, const TextToSpeechEnqueueInfo &info);

private:
    QMap<qsizetype, TextToSpeechEnqueueInfo> mEnqueueList;
};
