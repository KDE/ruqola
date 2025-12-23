/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plugintextinterface.h"

PluginTextInterface::PluginTextInterface(QWidget *parentWidget, QObject *parent)
    : QObject(parent)
    , mParentWidget(parentWidget)
{
}

PluginTextInterface::~PluginTextInterface() = default;

#include "moc_plugintextinterface.cpp"
