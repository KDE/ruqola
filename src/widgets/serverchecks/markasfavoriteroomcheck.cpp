/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "markasfavoriteroomcheck.h"
#include <KLocalizedString>

QString MarkAsFavoriteRoomCheck::name() const
{
    return i18n("Change Favorite Room State");
}

void MarkAsFavoriteRoomCheck::doStart()
{
    if (!mAccount) {
        reportFailed(i18n("No account."));
        return;
    }
    // TODO
}

void MarkAsFavoriteRoomCheck::checkState()
{
    // TODO
}

#include "moc_markasfavoriteroomcheck.cpp"
