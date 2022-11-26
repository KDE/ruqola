/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedittexttospeech_export.h"
#include <KPIMTextEditTextToSpeech/TextToSpeechWidget>
#include <QObject>
class QAction;
namespace KPIMTextEditTextToSpeech
{
class TextToSpeechActionsPrivate;
/**
 * @brief The TextToSpeechActions class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDITTEXTTOSPEECH_EXPORT TextToSpeechActions : public QObject
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
    void stateChanged(KPIMTextEditTextToSpeech::TextToSpeechWidget::State state);

private:
    void slotPlayPause();
    std::unique_ptr<TextToSpeechActionsPrivate> const d;
};
}
