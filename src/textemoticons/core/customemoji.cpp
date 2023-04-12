/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customemoji.h"
#include "emoticonunicodeutils.h"

using namespace TextEmoticonsCore;
CustomEmoji::CustomEmoji()
{
}

QString CustomEmoji::identifier() const
{
    return mIdentifier;
}

void CustomEmoji::setIdentifier(const QString &newIdentifier)
{
    mIdentifier = newIdentifier;
}

QString CustomEmoji::category() const
{
    return EmoticonUnicodeUtils::customIdentifier();
}

QDebug operator<<(QDebug d, const CustomEmoji &t)
{
    d << "Identifier " << t.identifier();
    return d;
}
