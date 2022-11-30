/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechinterface.h"
#include "texttospeech.h"
#include <QDebug>

using namespace KPIMTextEditTextToSpeech;

class Q_DECL_HIDDEN KPIMTextEditTextToSpeech::TextToSpeechInterfacePrivate
{
public:
    TextToSpeechInterfacePrivate(TextToSpeechWidget *textToSpeechWidget)
        : mTextToSpeechWidget(textToSpeechWidget)
    {
    }

    TextToSpeechWidget *const mTextToSpeechWidget;
};

TextToSpeechInterface::TextToSpeechInterface(TextToSpeechWidget *textToSpeechWidget, QObject *parent)
    : AbstractTextToSpeechInterface(parent)
    , d(new KPIMTextEditTextToSpeech::TextToSpeechInterfacePrivate(textToSpeechWidget))
{
    KPIMTextEditTextToSpeech::TextToSpeech::self(); // init
    connect(d->mTextToSpeechWidget, &KPIMTextEditTextToSpeech::TextToSpeechWidget::stateChanged, this, &TextToSpeechInterface::stateChanged);
    connect(KPIMTextEditTextToSpeech::TextToSpeech::self(),
            &KPIMTextEditTextToSpeech::TextToSpeech::stateChanged,
            d->mTextToSpeechWidget,
            &KPIMTextEditTextToSpeech::TextToSpeechWidget::slotStateChanged);
}

TextToSpeechInterface::~TextToSpeechInterface() = default;

bool TextToSpeechInterface::isReady() const
{
    return KPIMTextEditTextToSpeech::TextToSpeech::self()->isReady();
}

void TextToSpeechInterface::say(const QString &text)
{
    d->mTextToSpeechWidget->setState(KPIMTextEditTextToSpeech::TextToSpeechWidget::Play);
    d->mTextToSpeechWidget->show();
    KPIMTextEditTextToSpeech::TextToSpeech::self()->say(text);
}

double TextToSpeechInterface::volume() const
{
    return KPIMTextEditTextToSpeech::TextToSpeech::self()->volume();
}

void TextToSpeechInterface::setVolume(double value)
{
    KPIMTextEditTextToSpeech::TextToSpeech::self()->setVolume(value);
}

void TextToSpeechInterface::reloadSettings()
{
    KPIMTextEditTextToSpeech::TextToSpeech::self()->reloadSettings();
}

void TextToSpeechInterface::stateChanged(TextToSpeechWidget::State state)
{
    switch (state) {
    case TextToSpeechWidget::Stop:
        KPIMTextEditTextToSpeech::TextToSpeech::self()->stop();
        break;
    case TextToSpeechWidget::Play:
        KPIMTextEditTextToSpeech::TextToSpeech::self()->resume();
        break;
    case TextToSpeechWidget::Pause:
        KPIMTextEditTextToSpeech::TextToSpeech::self()->pause();
        break;
    }
}
