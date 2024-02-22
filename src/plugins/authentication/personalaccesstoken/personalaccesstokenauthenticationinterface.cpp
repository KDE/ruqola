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
        const PersonalAccessTokenPluginUtil::PersonalAccessTokenPluginInfo info = dlg->info();

        // TODO
    }
    delete dlg;
}

bool PersonalAccessTokenAuthenticationInterface::showConfigureDialog(QWidget *parent)
{
    // TODO
    return false;
}

#include "moc_personalaccesstokenauthenticationinterface.cpp"
