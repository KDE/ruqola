/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "textedittexttospeech_private_export.h"
#include <QObject>
namespace TextEditTextToSpeech
{
namespace TextToSpeechUtil
{
TEXTEDITTEXTTOSPEECH_TESTS_EXPORT Q_REQUIRED_RESULT QString textToSpeechConfigFileName();
TEXTEDITTEXTTOSPEECH_TESTS_EXPORT Q_REQUIRED_RESULT QString textToSpeechConfigGroupName();
};
}
