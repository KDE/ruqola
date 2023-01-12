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
        mainLayout->setObjectName(QStringLiteral("mainLayout"));
        mainLayout->setContentsMargins({});
    }
    TextToSpeechWidget *textToSpeechWidget = nullptr;
    QHBoxLayout *const mainLayout;
};

TextToSpeechContainerWidget::TextToSpeechContainerWidget(QWidget *parent)
    : QWidget{parent}
    , d(new TextEditTextToSpeech::TextToSpeechContainerWidgetPrivate(this))
{
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    hide();
}

TextToSpeechContainerWidget::~TextToSpeechContainerWidget() = default;

void TextToSpeechContainerWidget::say(const QString &text)
{
    if (!d->textToSpeechWidget) {
        d->textToSpeechWidget = new TextToSpeechWidget(this);
        connect(d->textToSpeechWidget, &TextToSpeechWidget::changeVisibility, this, &TextToSpeechContainerWidget::setVisible);
        d->mainLayout->addWidget(d->textToSpeechWidget);
    }
    d->textToSpeechWidget->say(text);
}
