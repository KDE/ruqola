/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechactions.h"
#include <KLocalizedString>
#include <QAction>

using namespace KPIMTextEditTextToSpeech;

class Q_DECL_HIDDEN KPIMTextEditTextToSpeech::TextToSpeechActionsPrivate
{
public:
    TextToSpeechActionsPrivate() = default;

    void updateButtonState();
    TextToSpeechWidget::State mState = TextToSpeechWidget::Stop;
    QAction *mStopAction = nullptr;
    QAction *mPlayPauseAction = nullptr;
};

TextToSpeechActions::TextToSpeechActions(QObject *parent)
    : QObject(parent)
    , d(new KPIMTextEditTextToSpeech::TextToSpeechActionsPrivate)
{
    d->mStopAction = new QAction(i18n("Stop"), this);
    d->mStopAction->setObjectName(QStringLiteral("stopbutton"));
    d->mStopAction->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-stop")));
    d->mStopAction->setToolTip(i18n("Stop"));
    connect(d->mStopAction, &QAction::triggered, this, &TextToSpeechActions::slotStop);

    d->mPlayPauseAction = new QAction(this);
    d->mPlayPauseAction->setObjectName(QStringLiteral("playpausebutton"));
    d->mPlayPauseAction->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
    connect(d->mPlayPauseAction, &QAction::triggered, this, &TextToSpeechActions::slotPlayPause);

    d->updateButtonState();
}

TextToSpeechActions::~TextToSpeechActions() = default;

QAction *TextToSpeechActions::stopAction() const
{
    return d->mStopAction;
}

QAction *TextToSpeechActions::playPauseAction() const
{
    return d->mPlayPauseAction;
}

TextToSpeechWidget::State TextToSpeechActions::state() const
{
    return d->mState;
}

void TextToSpeechActions::setState(TextToSpeechWidget::State state)
{
    if (d->mState != state) {
        d->mState = state;
        d->updateButtonState();
    }
}

void TextToSpeechActionsPrivate::updateButtonState()
{
    mPlayPauseAction->setIcon(
        QIcon::fromTheme((mState == TextToSpeechWidget::Stop) ? QStringLiteral("media-playback-start") : QStringLiteral("media-playback-pause")));
    mPlayPauseAction->setEnabled((mState != TextToSpeechWidget::Stop));
    const QString text = (mState != TextToSpeechWidget::Play) ? i18n("Pause") : i18n("Play");
    mPlayPauseAction->setToolTip(text);
    mPlayPauseAction->setText(text);
}

void TextToSpeechActions::slotPlayPause()
{
    if (d->mState == KPIMTextEditTextToSpeech::TextToSpeechWidget::Pause) {
        d->mState = KPIMTextEditTextToSpeech::TextToSpeechWidget::Play;
    } else if (d->mState == KPIMTextEditTextToSpeech::TextToSpeechWidget::Play) {
        d->mState = KPIMTextEditTextToSpeech::TextToSpeechWidget::Pause;
    } else if (d->mState == KPIMTextEditTextToSpeech::TextToSpeechWidget::Stop) {
        d->mState = KPIMTextEditTextToSpeech::TextToSpeechWidget::Play;
    } else {
        return;
    }
    d->updateButtonState();
    Q_EMIT stateChanged(d->mState);
}

void TextToSpeechActions::slotStop()
{
    if (d->mState != KPIMTextEditTextToSpeech::TextToSpeechWidget::Stop) {
        d->mState = KPIMTextEditTextToSpeech::TextToSpeechWidget::Stop;
        d->updateButtonState();
        Q_EMIT stateChanged(d->mState);
    }
}
