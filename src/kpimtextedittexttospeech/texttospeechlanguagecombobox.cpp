/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechlanguagecombobox.h"
using namespace KPIMTextEditTextToSpeech;

TextToSpeechLanguageComboBox::TextToSpeechLanguageComboBox(QWidget *parent)
    : QComboBox(parent)
{
}

TextToSpeechLanguageComboBox::~TextToSpeechLanguageComboBox() = default;

void TextToSpeechLanguageComboBox::selectLocaleName(const QString &localeName)
{
    const int countItem(count());
    for (int i = 0; i < countItem; ++i) {
        if (itemData(i).toLocale().name() == localeName) {
            setCurrentIndex(i);
            break;
        }
    }
}

void TextToSpeechLanguageComboBox::updateAvailableLocales(const QVector<QLocale> &locales, const QLocale &current)
{
    clear();
    for (const QLocale &locale : locales) {
        const QVariant localeVariant(locale);
        addItem(QLocale::languageToString(locale.language()), localeVariant);
        if (locale.name() == current.name()) {
            setCurrentIndex(count() - 1);
        }
    }
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
}
