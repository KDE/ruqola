/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "grabscreenplugintool.h"
using namespace Qt::Literals::StringLiterals;

#include "grabscreenplugintoolconfiguredialog.h"
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
    return Utils::executableFound(u"spectacle"_s);
}

PluginTool::ToolType GrabScreenPluginTool::toolType() const
{
    return PluginTool::ToolType::MessageViewToolBar;
}

QString GrabScreenPluginTool::iconName() const
{
    return u"transform-crop"_s;
}

QString GrabScreenPluginTool::toolTip() const
{
    return i18n("Create a screenshot");
}

QString GrabScreenPluginTool::description() const
{
    return i18n("This tool allows to create screenshot.");
}

PluginToolInterface *GrabScreenPluginTool::createInterface(QWidget *parentWidget, QObject *parent)
{
    return new GrabScreenPluginToolInterface(parentWidget, parent);
}

int GrabScreenPluginTool::order() const
{
    return 20;
}

bool GrabScreenPluginTool::hasMenu() const
{
    return false;
}

bool GrabScreenPluginTool::hasConfigureDialog() const
{
    return true;
}

void GrabScreenPluginTool::showConfigureDialog(QWidget *parent) const
{
    GrabScreenPluginToolConfigureDialog d(parent);
    d.exec();
    // TODO
}
#include "grabscreenplugintool.moc"

#include "moc_grabscreenplugintool.cpp"
