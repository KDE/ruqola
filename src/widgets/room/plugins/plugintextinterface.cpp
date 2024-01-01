/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plugintextinterface.h"

PluginTextInterface::PluginTextInterface(QObject *parent)
    : QObject(parent)
{
}

PluginTextInterface::~PluginTextInterface() = default;

void PluginTextInterface::setSelectedText(const QString &str)
{
    Q_UNUSED(str)
}

#include "moc_plugintextinterface.cpp"
