/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechutil.h"
#include <KConfig>

KConfigGroup KPIMTextEditTextToSpeech::TextToSpeechUtil::textToSpeechConfigGroup()
{
    KConfig config(QStringLiteral("texttospeechrc"));
    KConfigGroup grp = config.group("Settings");
    return grp;
}
