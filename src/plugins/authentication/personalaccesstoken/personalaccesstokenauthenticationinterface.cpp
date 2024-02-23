/*
   SPDX-FileCopyrightText:  2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokenauthenticationinterface.h"
#include "personalaccesstokenauthenticationconfigwidget.h"
#include "rocketchataccount.h"
#include <QPointer>

PersonalAccessTokenAuthenticationInterface::PersonalAccessTokenAuthenticationInterface(QObject *parent)
    : PluginAuthenticationInterface(parent)
{
}

PersonalAccessTokenAuthenticationInterface::~PersonalAccessTokenAuthenticationInterface() = default;

void PersonalAccessTokenAuthenticationInterface::login()
{
    // TODO
}

PluginAuthenticationConfigureWidget *PersonalAccessTokenAuthenticationInterface::configureWidget(QWidget *parent)
{
    return new PersonalAccessTokenAuthenticationConfigWidget(parent);
}

#include "moc_personalaccesstokenauthenticationinterface.cpp"
