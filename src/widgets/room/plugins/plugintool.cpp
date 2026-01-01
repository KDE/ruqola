/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plugintool.h"

PluginTool::PluginTool(QObject *parent)
    : PluginBase{parent}
{
}

PluginTool::~PluginTool() = default;

#include "moc_plugintool.cpp"
