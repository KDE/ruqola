/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechinterface.h"
#include "texttospeech.h"
#include <QDebug>

using namespace TextEditTextToSpeech;

class Q_DECL_HIDDEN TextEditTextToSpeech::TextToSpeechInterfacePrivate
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
    , d(new TextEditTextToSpeech::TextToSpeechInterfacePrivate(textToSpeechWidget))
{
    TextEditTextToSpeech::TextToSpeech::self(); // init
    connect(d->mTextToSpeechWidget, &TextEditTextToSpeech::TextToSpeechWidget::stateChanged, this, &TextToSpeechInterface::stateChanged);
    connect(TextEditTextToSpeech::TextToSpeech::self(),
            &TextEditTextToSpeech::TextToSpeech::stateChanged,
            d->mTextToSpeechWidget,
            &TextEditTextToSpeech::TextToSpeechWidget::slotStateChanged);
}

TextToSpeechInterface::~TextToSpeechInterface() = default;

bool TextToSpeechInterface::isReady() const
{
    return TextEditTextToSpeech::TextToSpeech::self()->isReady();
}

void TextToSpeechInterface::say(const QString &text)
{
    d->mTextToSpeechWidget->setState(TextEditTextToSpeech::TextToSpeechWidget::Play);
    d->mTextToSpeechWidget->show();
    TextEditTextToSpeech::TextToSpeech::self()->say(text);
}

double TextToSpeechInterface::volume() const
{
    return TextEditTextToSpeech::TextToSpeech::self()->volume();
}

void TextToSpeechInterface::setVolume(double value)
{
    TextEditTextToSpeech::TextToSpeech::self()->setVolume(value);
}

void TextToSpeechInterface::reloadSettings()
{
    TextEditTextToSpeech::TextToSpeech::self()->reloadSettings();
}

void TextToSpeechInterface::stateChanged(TextToSpeechWidget::State state)
{
    switch (state) {
    case TextToSpeechWidget::Stop:
        TextEditTextToSpeech::TextToSpeech::self()->stop();
        break;
    case TextToSpeechWidget::Play:
        TextEditTextToSpeech::TextToSpeech::self()->resume();
        break;
    case TextToSpeechWidget::Pause:
        TextEditTextToSpeech::TextToSpeech::self()->pause();
        break;
    }
}
