/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogeneratetextplugintool.h"
#include "autogeneratetextplugintoolinterface.h"
#include <KLocalizedString>
#include <KPluginFactory>
#include <QStandardPaths>

K_PLUGIN_CLASS_WITH_JSON(AutoGenerateTextPluginTool, "ruqola_autogeneratetexttoolsplugin.json")

AutoGenerateTextPluginTool::AutoGenerateTextPluginTool(QObject *parent, const QVariantList &)
    : PluginTool{parent}
{
}

AutoGenerateTextPluginTool::~AutoGenerateTextPluginTool() = default;

bool AutoGenerateTextPluginTool::toolFound() const
{
    return true;
}

PluginTool::ToolType AutoGenerateTextPluginTool::toolType() const
{
    return PluginTool::ToolType::MessageViewToolBar;
}

QString AutoGenerateTextPluginTool::iconName() const
{
    return QStringLiteral("question"); // TODO change it
}

QString AutoGenerateTextPluginTool::toolTip() const
{
    return i18n("Ask to AI");
}

QString AutoGenerateTextPluginTool::description() const
{
    return {};
}

PluginToolInterface *AutoGenerateTextPluginTool::createInterface(QObject *parent)
{
    return new AutoGenerateTextPluginToolInterface(parent);
}

int AutoGenerateTextPluginTool::order() const
{
    return 30;
}

bool AutoGenerateTextPluginTool::hasMenu() const
{
    return false;
}

bool AutoGenerateTextPluginTool::hasConfigureDialog() const
{
    return false;
}

#include "autogeneratetextplugintool.moc"

#include "moc_autogeneratetextplugintool.cpp"
