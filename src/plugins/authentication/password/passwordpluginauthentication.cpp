/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "passwordpluginauthentication.h"
#include "passwordauthenticationinterface.h"
#include <kpluginfactory.h>
#include <KLocalizedString>
#include "plugins/kcoreaddons_ruqola_compat.h"

K_PLUGIN_CLASS_WITH_JSON(PasswordPluginAuthentication, "ruqola_passwordauthenticationplugin.json")

PasswordPluginAuthentication::PasswordPluginAuthentication(QObject *parent, const QVariantList&)
    : PluginAuthentication(parent)
{

}

PasswordPluginAuthentication::~PasswordPluginAuthentication()
{
}

PluginAuthenticationInterface *PasswordPluginAuthentication::createInterface(QObject *parent)
{
    return new PasswordAuthenticationInterface(parent);
}

AuthenticationManager::OauthType PasswordPluginAuthentication::type() const
{
    return AuthenticationManager::OauthType::Password;
}

QString PasswordPluginAuthentication::name() const
{
    return i18n("Login/Password");
}

QString PasswordPluginAuthentication::iconName() const
{
    //TODO
    return {};
}

#include "passwordpluginauthentication.moc"
