/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plugintool.h"

PluginTool::PluginTool(QObject *parent)
    : QObject{parent}
{
}

PluginTool::~PluginTool() = default;

QMenu *PluginTool::menu() const
{
    return nullptr;
}

#include "moc_plugintool.cpp"
