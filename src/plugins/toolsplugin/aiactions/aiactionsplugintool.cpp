/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "aiactionsplugintool.h"

#include "aiactionsplugintoolinterface.h"
#include <KLocalizedString>
#include <KPluginFactory>
#include <QStandardPaths>

using namespace Qt::Literals::StringLiterals;
K_PLUGIN_CLASS_WITH_JSON(AiActionsPluginTool, "ruqola_aiactionstoolsplugin.json")
AiActionsPluginTool::AiActionsPluginTool(QObject *parent, const QVariantList &)
    : PluginTool{parent}
{
}

AiActionsPluginTool::~AiActionsPluginTool() = default;

bool AiActionsPluginTool::toolFound() const
{
    return true;
}

PluginTool::ToolType AiActionsPluginTool::toolType() const
{
    return PluginTool::ToolType::MessageViewHeaderToolBar;
}

QString AiActionsPluginTool::iconName() const
{
    return u"question"_s; // TODO change it
}

QString AiActionsPluginTool::toolTip() const
{
    return i18n("AI Actions");
}

QString AiActionsPluginTool::description() const
{
    return {};
}

PluginToolInterface *AiActionsPluginTool::createInterface(QWidget *parentWidget, QObject *parent)
{
    return new AiActionsPluginToolInterface(parentWidget, parent);
}

int AiActionsPluginTool::order() const
{
    return 10;
}

bool AiActionsPluginTool::hasMenu() const
{
    return true;
}

bool AiActionsPluginTool::hasConfigureDialog() const
{
    return false;
}

#include "aiactionsplugintool.moc"
#include "moc_aiactionsplugintool.cpp"
