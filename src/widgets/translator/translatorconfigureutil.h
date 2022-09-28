/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "translatetext/translatorenginebase.h"

class QComboBox;
namespace TranslatorConfigureUtil
{
void fillComboboxSettings(QComboBox *combo);
Q_REQUIRED_RESULT bool hasConfigureDialog(TranslatorEngineBase::TranslatorEngine engineType);
}
