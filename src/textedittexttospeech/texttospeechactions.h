/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "textedittexttospeech_export.h"
#include <QObject>
#include <TextEditTextToSpeech/TextToSpeechWidget>
class QAction;
namespace TextEditTextToSpeech
{
class TextToSpeechActionsPrivate;
/**
 * @brief The TextToSpeechActions class
 * @author Laurent Montel <montel@kde.org>
 */
class TEXTEDITTEXTTOSPEECH_EXPORT TextToSpeechActions : public QObject
{
    Q_OBJECT
public:
    explicit TextToSpeechActions(QObject *parent = nullptr);
    ~TextToSpeechActions() override;

    QAction *stopAction() const;

    QAction *playPauseAction() const;

    Q_REQUIRED_RESULT TextToSpeechWidget::State state() const;

public Q_SLOTS:
    void setState(TextToSpeechWidget::State);
    void slotStop();

Q_SIGNALS:
    void stateChanged(TextEditTextToSpeech::TextToSpeechWidget::State state);

private:
    void slotPlayPause();
    std::unique_ptr<TextToSpeechActionsPrivate> const d;
};
}
