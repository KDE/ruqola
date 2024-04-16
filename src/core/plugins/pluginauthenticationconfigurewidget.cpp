/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "pluginauthenticationconfigurewidget.h"

PluginAuthenticationConfigureWidget::PluginAuthenticationConfigureWidget(QWidget *parent)
    : QWidget{parent}
{
}

PluginAuthenticationConfigureWidget::~PluginAuthenticationConfigureWidget() = default;

void PluginAuthenticationConfigureWidget::setExistingAccountNames(const QStringList &list)
{
    mExistingAccountNames = list;
}

void PluginAuthenticationConfigureWidget::setAuthenticationLoginType(AuthenticationLoginType type)
{
    Q_UNUSED(type);
}

#include "moc_pluginauthenticationconfigurewidget.cpp"
