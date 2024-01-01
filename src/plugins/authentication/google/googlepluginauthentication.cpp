/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "googlepluginauthentication.h"
#include "googleauthenticationinterface.h"
#include "plugins/kcoreaddons_ruqola_compat.h"
#include <KLocalizedString>
#include <KPluginFactory>

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
    // TODO
    return {};
}

#include "googlepluginauthentication.moc"

#include "moc_googlepluginauthentication.cpp"
