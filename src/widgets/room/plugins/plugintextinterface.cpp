/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
