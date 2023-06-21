/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plugintext.h"

PluginText::PluginText(QObject *parent)
    : QObject(parent)
{
}

PluginText::~PluginText() = default;

#include "moc_plugintext.cpp"
