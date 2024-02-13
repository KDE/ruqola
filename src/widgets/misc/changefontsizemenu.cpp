/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changefontsizemenu.h"
#include <KLocalizedString>
#include <QMenu>

ChangeFontSizeMenu::ChangeFontSizeMenu(QObject *parent)
    : KActionMenu{parent}
{
}

ChangeFontSizeMenu::~ChangeFontSizeMenu() = default;

#include "moc_changefontsizemenu.cpp"
