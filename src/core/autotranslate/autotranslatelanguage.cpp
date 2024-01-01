/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autotranslatelanguage.h"

AutotranslateLanguage::AutotranslateLanguage() = default;

QString AutotranslateLanguage::language() const
{
    return mLanguage;
}

void AutotranslateLanguage::setLanguage(const QString &language)
{
    mLanguage = language;
}

QString AutotranslateLanguage::displayLanguage() const
{
    return mDisplayLanguage.isEmpty() ? mLanguage : mDisplayLanguage;
}

void AutotranslateLanguage::setDisplayLanguage(const QString &displayLanguage)
{
    mDisplayLanguage = displayLanguage;
}

QDebug operator<<(QDebug d, const AutotranslateLanguage &t)
{
    d.space() << "language name:" << t.language() << "\n";
    d.space() << "Display language name:" << t.displayLanguage() << "\n";
    return d;
}
