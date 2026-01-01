/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordpluginauthentication.h"
using namespace Qt::Literals::StringLiterals;

#include "passwordauthenticationinterface.h"
#include <KLocalizedString>
#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(PasswordPluginAuthentication, "ruqola_passwordauthenticationplugin.json")

PasswordPluginAuthentication::PasswordPluginAuthentication(QObject *parent, const QVariantList &)
    : PluginAuthentication(parent)
{
}

PasswordPluginAuthentication::~PasswordPluginAuthentication() = default;

PluginAuthenticationInterface *PasswordPluginAuthentication::createInterface(QObject *parent)
{
    return new PasswordAuthenticationInterface(parent);
}

AuthenticationManager::AuthMethodType PasswordPluginAuthentication::authenticationType() const
{
    return AuthenticationManager::AuthMethodType::Password;
}

QString PasswordPluginAuthentication::name() const
{
    return i18n("Login/Password");
}

QString PasswordPluginAuthentication::iconName() const
{
    return u"dialog-password"_s;
}

#include "passwordpluginauthentication.moc"

#include "moc_passwordpluginauthentication.cpp"
