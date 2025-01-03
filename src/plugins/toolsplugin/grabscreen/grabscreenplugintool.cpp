/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "grabscreenplugintool.h"
#include "grabscreenplugintoolinterface.h"
#include "utils.h"
#include <KLocalizedString>
#include <KPluginFactory>
#include <QStandardPaths>

K_PLUGIN_CLASS_WITH_JSON(GrabScreenPluginTool, "ruqola_grabscreentoolsplugin.json")

GrabScreenPluginTool::GrabScreenPluginTool(QObject *parent, const QVariantList &)
    : PluginTool{parent}
{
}

GrabScreenPluginTool::~GrabScreenPluginTool() = default;

bool GrabScreenPluginTool::toolFound() const
{
    const QString path = Utils::findExecutable(QStringLiteral("spectacle"));
    return !path.isEmpty();
}

PluginTool::ToolType GrabScreenPluginTool::toolType() const
{
    return PluginTool::ToolType::MessageViewToolBar;
}

QString GrabScreenPluginTool::iconName() const
{
    return QStringLiteral("transform-crop");
}

QString GrabScreenPluginTool::toolTip() const
{
    return i18n("Create a screenshot");
}

QString GrabScreenPluginTool::description() const
{
    return {};
}

PluginToolInterface *GrabScreenPluginTool::createInterface(QObject *parent)
{
    return new GrabScreenPluginToolInterface(parent);
}
#include "grabscreenplugintool.moc"

#include "moc_grabscreenplugintool.cpp"
