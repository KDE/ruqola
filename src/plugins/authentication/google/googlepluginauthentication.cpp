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

#include "googlepluginauthentication.h"
#include "googleauthenticationinterface.h"
#include <kpluginfactory.h>
#include <KLocalizedString>
#include "plugins/kcoreaddons_ruqola_compat.h"

K_PLUGIN_CLASS_WITH_JSON(GooglePluginAuthentication, "ruqola_googleauthenticationplugin.json")

GooglePluginAuthentication::GooglePluginAuthentication(QObject *parent, const QVariantList &)
    : PluginAuthentication(parent)
{
}

GooglePluginAuthentication::~GooglePluginAuthentication()
{
}

PluginAuthenticationInterface *GooglePluginAuthentication::createInterface(QObject *parent)
{
    return new GoogleAuthenticationInterface(parent);
}

AuthenticationManager::OauthType GooglePluginAuthentication::type() const
{
    return AuthenticationManager::OauthType::Google;
}

QString GooglePluginAuthentication::name() const
{
    return i18n("Google");
}

QString GooglePluginAuthentication::iconName() const
{
    //TODO
    return {};
}

#include "googlepluginauthentication.moc"
