/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageextra.h"

MessageExtra::MessageExtra() = default;

QString MessageExtra::localTranslation() const
{
    return mLocalTranslation;
}

void MessageExtra::setLocalTranslation(const QString &newLocalTranslation)
{
    mLocalTranslation = newLocalTranslation;
}
