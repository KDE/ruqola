/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatormenu.h"

#include <QMenu>

TranslatorMenu::TranslatorMenu(QObject *parent)
    : QObject(parent)
    , mMenu(new QMenu)
{
    mMenu->setObjectName(QStringLiteral("menu"));
}

TranslatorMenu::~TranslatorMenu()
{
    delete mMenu;
}

void TranslatorMenu::updateMenu()
{
    mMenu->clear();
    // TODO
}

QMenu *TranslatorMenu::menu() const
{
    return mMenu;
}
