/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechutil.h"

QString KPIMTextEditTextToSpeech::TextToSpeechUtil::textToSpeechConfigFileName()
{
    return QStringLiteral("texttospeechrc");
}

QString KPIMTextEditTextToSpeech::TextToSpeechUtil::textToSpeechConfigGroupName()
{
    return QStringLiteral("Settings");
}
