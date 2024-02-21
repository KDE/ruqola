/*
   SPDX-FileCopyrightText:  2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokenauthenticationinterface.h"
#include "personalaccesstokenauthenticationconfigdialog.h"
#include "personalaccesstokenpluginutil.h"
#include "rocketchataccount.h"
#include <QPointer>

PersonalAccessTokenAuthenticationInterface::PersonalAccessTokenAuthenticationInterface(QObject *parent)
    : PluginAuthenticationInterface(parent)
{
}

PersonalAccessTokenAuthenticationInterface::~PersonalAccessTokenAuthenticationInterface() = default;

void PersonalAccessTokenAuthenticationInterface::login()
{
    // TODO add widget ?
    QPointer<PersonalAccessTokenAuthenticationConfigDialog> dlg = new PersonalAccessTokenAuthenticationConfigDialog(nullptr);
    if (dlg->exec()) {
        // TODO
    }
    delete dlg;
    // TODO
}

#include "moc_personalaccesstokenauthenticationinterface.cpp"
