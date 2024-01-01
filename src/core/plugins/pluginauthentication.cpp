/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "pluginauthentication.h"

PluginAuthentication::PluginAuthentication(QObject *parent)
    : QObject(parent)
{
}

PluginAuthentication::~PluginAuthentication() = default;

#include "moc_pluginauthentication.cpp"
