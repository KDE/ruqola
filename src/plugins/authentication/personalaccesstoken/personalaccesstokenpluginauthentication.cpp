/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokenpluginauthentication.h"
using namespace Qt::Literals::StringLiterals;

#include "personalaccesstokenauthenticationinterface.h"
#include <KLocalizedString>
#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(PersonalAccessTokenPluginAuthentication, "ruqola_personalaccesstokenauthenticationplugin.json")

PersonalAccessTokenPluginAuthentication::PersonalAccessTokenPluginAuthentication(QObject *parent, const QVariantList &)
    : PluginAuthentication(parent)
{
}

PersonalAccessTokenPluginAuthentication::~PersonalAccessTokenPluginAuthentication() = default;

PluginAuthenticationInterface *PersonalAccessTokenPluginAuthentication::createInterface(QObject *parent)
{
    return new PersonalAccessTokenAuthenticationInterface(parent);
}

AuthenticationManager::AuthMethodType PersonalAccessTokenPluginAuthentication::authenticationType() const
{
    return AuthenticationManager::AuthMethodType::PersonalAccessToken;
}

QString PersonalAccessTokenPluginAuthentication::name() const
{
    return i18n("Personal Access Token");
}

QString PersonalAccessTokenPluginAuthentication::iconName() const
{
    return u"dialog-password"_s;
}

#include "moc_personalaccesstokenpluginauthentication.cpp"
#include "personalaccesstokenpluginauthentication.moc"
