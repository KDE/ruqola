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
    setText(i18n("Font Size"));

    auto increaseFontSize = new QAction(i18n("Increase Font"), this);
    menu()->addAction(increaseFontSize);
    connect(increaseFontSize, &QAction::triggered, this, [this]() {});
    auto decreaseFontSize = new QAction(i18n("Increase Font"), this);
    menu()->addAction(decreaseFontSize);
    connect(decreaseFontSize, &QAction::triggered, this, [this]() {});
}

ChangeFontSizeMenu::~ChangeFontSizeMenu() = default;

#include "moc_changefontsizemenu.cpp"
