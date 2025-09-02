/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "pluginbase.h"

PluginBase::PluginBase(QObject *parent)
    : QObject{parent}
{
}

PluginBase::~PluginBase() = default;

bool PluginBase::enabled() const
{
    return mEnabled;
}

void PluginBase::setEnabled(bool newEnabled)
{
    mEnabled = newEnabled;
}

bool PluginBase::hasConfigureDialog() const
{
    return false;
}

void PluginBase::showConfigureDialog([[maybe_unused]] QWidget *parent) const
{
}

#include "moc_pluginbase.cpp"
