/*
   SPDX-FileCopyrightText:  2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokenauthenticationinterface.h"
#include "personalaccesstokenauthenticationconfigdialog.h"
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

bool PersonalAccessTokenAuthenticationInterface::showConfigureDialog(QWidget *parent)
{
    bool result = false;
    QPointer<PersonalAccessTokenAuthenticationConfigDialog> dlg = new PersonalAccessTokenAuthenticationConfigDialog(nullptr);
    if (dlg->exec()) {
        result = true;
        // TODO
    }
    delete dlg;
    return result;
}

#include "moc_personalaccesstokenauthenticationinterface.cpp"
