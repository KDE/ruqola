/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagestylelayoutmenu.h"
#include <KLocalizedString>

MessageStyleLayoutMenu::MessageStyleLayoutMenu(QObject *parent)
    : KActionMenu{parent}
{
    setText(i18n("Message Style"));
}

MessageStyleLayoutMenu::~MessageStyleLayoutMenu() = default;

#include "moc_messagestylelayoutmenu.cpp"
