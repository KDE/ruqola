/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plugintoolinterface.h"

PluginToolInterface::PluginToolInterface(QWidget *parentWidget, QObject *parent)
    : QObject{parent}
    , mParentWidget(parentWidget)
{
}

PluginToolInterface::~PluginToolInterface() = default;

void PluginToolInterface::activateTool()
{
    // Nothing here. Reimplement it
}

PluginToolInterface::PluginToolInfo PluginToolInterface::info() const
{
    return mInfo;
}

void PluginToolInterface::setInfo(const PluginToolInfo &newInfo)
{
    mInfo = newInfo;
}

QMenu *PluginToolInterface::menu() const
{
    return nullptr;
}

QDebug operator<<(QDebug d, const PluginToolInterface::PluginToolInfo &t)
{
    d.space() << "roomId:" << t.roomId;
    d.space() << "accountName:" << t.accountName;
    d.space() << "tmid:" << t.tmid;
    d.space() << "msgId:" << t.msgId;
    return d;
}

#include "moc_plugintoolinterface.cpp"
