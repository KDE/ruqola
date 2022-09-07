/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatormenu.h"

#include <KConfigGroup>
#include <KSharedConfig>
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
    KConfigGroup groupTranslate(KSharedConfig::openConfig(), QStringLiteral("Translate"));
    const auto fromList = groupTranslate.readEntry(QStringLiteral("From"), QStringList());
    const auto toList = groupTranslate.readEntry(QStringLiteral("To"), QStringList());

    for (const auto &fromLang : fromList) {
        for (const auto &toLang : toList) {
            // TODO
        }
    }
}

QMenu *TranslatorMenu::menu() const
{
    return mMenu;
}
