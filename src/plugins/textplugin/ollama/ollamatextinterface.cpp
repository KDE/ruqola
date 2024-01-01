/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ollamatextinterface.h"

#include <QMenu>

OllamaTextInterface::OllamaTextInterface(QObject *parent)
    : PluginTextInterface(parent)
{
}

OllamaTextInterface::~OllamaTextInterface() = default;

void OllamaTextInterface::addAction(QMenu *menu)
{
    menu->addSeparator();
    // TODO
}

void OllamaTextInterface::setSelectedText(const QString &str)
{
    // TODO
}

#include "moc_ollamatextinterface.cpp"
