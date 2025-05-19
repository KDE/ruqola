/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "aitextinterface.h"
#include <TextAutoGenerateText/TextAutoGenerateMenuWidget>

#include <QMenu>

AiTextInterface::AiTextInterface(QObject *parent)
    : PluginTextInterface(parent)
    , mMenuWidget(new TextAutoGenerateText::TextAutoGenerateMenuWidget(this))
{
}

AiTextInterface::~AiTextInterface() = default;

void AiTextInterface::addAction(QMenu *menu)
{
    menu->addSeparator();
    menu->addMenu(mMenuWidget->menu());
}

void AiTextInterface::setSelectedText(const QString &str)
{
    mMenuWidget->setSelectedText(str);
}

#include "moc_aitextinterface.cpp"
