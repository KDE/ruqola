/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "room/plugins/plugintextinterface.h"
class QTextToSpeech;
class TextToSpeechTextInterface : public PluginTextInterface
{
    Q_OBJECT
public:
    explicit TextToSpeechTextInterface(QObject *parent = nullptr);
    ~TextToSpeechTextInterface() override;

    void addAction(QMenu *menu) override;

    void setSelectedText(const QString &str) override;

private:
    void slotSpeakText();
    QString mSelectedText;
};
