/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "grabscreenplugintool.h"
#include "grabscreenplugintoolinterface.h"

GrabScreenPluginTool::GrabScreenPluginTool(QObject *parent)
    : PluginTool{parent}
{
}

GrabScreenPluginTool::~GrabScreenPluginTool() = default;

bool GrabScreenPluginTool::toolFound() const
{
    // TODO
    return true;
}

PluginTool::ToolType GrabScreenPluginTool::toolType() const
{
    return PluginTool::ToolType::MessageViewToolBar;
}

QString GrabScreenPluginTool::iconName() const
{
    return {};
}

QString GrabScreenPluginTool::toolTip() const
{
    return {};
}

QString GrabScreenPluginTool::description() const
{
    return {};
}

PluginToolInterface *GrabScreenPluginTool::createInterface(QObject *parent)
{
    return new GrabScreenPluginToolInterface(parent);
}
