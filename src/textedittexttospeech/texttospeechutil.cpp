/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechutil.h"

QString TextEditTextToSpeech::TextToSpeechUtil::textToSpeechConfigFileName()
{
    return QStringLiteral("texttospeechrc");
}

QString TextEditTextToSpeech::TextToSpeechUtil::textToSpeechConfigGroupName()
{
    return QStringLiteral("Settings");
}
