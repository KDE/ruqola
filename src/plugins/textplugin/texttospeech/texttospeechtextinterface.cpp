/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechtextinterface.h"
#include "texttospeechmanager.h"

#include <KLocalizedString>

#include <QAction>
#include <QIcon>
#include <QMenu>

#include <QTextToSpeech>

TextToSpeechTextInterface::TextToSpeechTextInterface(QObject *parent)
    : PluginTextInterface(parent)
{
}

TextToSpeechTextInterface::~TextToSpeechTextInterface() = default;

void TextToSpeechTextInterface::addAction(QMenu *menu)
{
    menu->addSeparator();
    QAction *speakAction = menu->addAction(QIcon::fromTheme(QStringLiteral("preferences-desktop-text-to-speech")), i18n("Speak Text"));
    connect(speakAction, &QAction::triggered, this, &TextToSpeechTextInterface::slotSpeakText);
}

void TextToSpeechTextInterface::setSelectedText(const QString &str)
{
    mSelectedText = str;
}

void TextToSpeechTextInterface::slotSpeakText()
{
    TextToSpeechManager::self()->textToSpeech()->say(mSelectedText);
}
