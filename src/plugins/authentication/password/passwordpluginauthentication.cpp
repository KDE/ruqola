/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordpluginauthentication.h"
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

AuthenticationManager::AuthMethodType PasswordPluginAuthentication::type() const
{
    return AuthenticationManager::AuthMethodType::Password;
}

QString PasswordPluginAuthentication::name() const
{
    return i18n("Login/Password");
}

QString PasswordPluginAuthentication::iconName() const
{
    return QStringLiteral("dialog-password");
}

#include "passwordpluginauthentication.moc"

#include "moc_passwordpluginauthentication.cpp"
