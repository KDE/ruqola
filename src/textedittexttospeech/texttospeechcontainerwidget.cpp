/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechcontainerwidget.h"
#include "texttospeechwidget.h"
#include <QHBoxLayout>
using namespace TextEditTextToSpeech;

class Q_DECL_HIDDEN TextEditTextToSpeech::TextToSpeechContainerWidgetPrivate
{
public:
    TextToSpeechContainerWidgetPrivate(TextToSpeechContainerWidget *q)
        : mainLayout(new QHBoxLayout(q))
    {
    }
    TextToSpeechWidget *textToSpeechWidget = nullptr;
    QHBoxLayout *const mainLayout;
};

TextToSpeechContainerWidget::TextToSpeechContainerWidget(QWidget *parent)
    : QWidget{parent}
    , d(new TextEditTextToSpeech::TextToSpeechContainerWidgetPrivate(this))
{
    hide();
}

TextToSpeechContainerWidget::~TextToSpeechContainerWidget() = default;

void TextToSpeechContainerWidget::say(const QString &text)
{
    if (!d->textToSpeechWidget) {
        d->textToSpeechWidget = new TextToSpeechWidget(this);
        connect(d->textToSpeechWidget, &TextToSpeechWidget::changeVisibility, this, &TextToSpeechContainerWidget::slotChangeVisibility);
        d->mainLayout->addWidget(d->textToSpeechWidget);
    }
    d->textToSpeechWidget->say(text);
}

void TextToSpeechContainerWidget::slotChangeVisibility(bool state)
{
    if (state) {
        show();
    } else {
        hide();
    }
}
