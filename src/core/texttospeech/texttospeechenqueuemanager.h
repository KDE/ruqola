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

    [[nodiscard]] bool contains(const TextToSpeechEnqueueInfo &info) const;

    void insert(const TextToSpeechEnqueueInfo &info);

    [[nodiscard]] TextToSpeechEnqueueInfo value(qsizetype index) const;

private:
    QList<TextToSpeechEnqueueInfo> mEnqueueList;
};
