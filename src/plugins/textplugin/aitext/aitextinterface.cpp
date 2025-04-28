/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "aitextinterface.h"

#include <QMenu>

AiTextInterface::AiTextInterface(QObject *parent)
    : PluginTextInterface(parent)
{
}

AiTextInterface::~AiTextInterface() = default;

void AiTextInterface::addAction(QMenu *menu)
{
    menu->addSeparator();
}

void AiTextInterface::setSelectedText(const QString &str)
{
}

#include "moc_aitextinterface.cpp"
