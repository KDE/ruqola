/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "githubpluginauthentication.h"
#include "githubauthenticationinterface.h"
#include <KLocalizedString>
#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(GitHubPluginAuthentication, "ruqola_githubauthenticationplugin.json")

GitHubPluginAuthentication::GitHubPluginAuthentication(QObject *parent, const QVariantList &)
    : PluginAuthentication(parent)
{
}

GitHubPluginAuthentication::~GitHubPluginAuthentication() = default;

PluginAuthenticationInterface *GitHubPluginAuthentication::createInterface(QObject *parent)
{
    return new GitHubAuthenticationInterface(parent);
}

AuthenticationManager::AuthMethodType GitHubPluginAuthentication::type() const
{
    return AuthenticationManager::AuthMethodType::GitHub;
}

QString GitHubPluginAuthentication::name() const
{
    return i18n("GitHub");
}

QString GitHubPluginAuthentication::iconName() const
{
    return QStringLiteral("dialog-password");
}

#include "githubpluginauthentication.moc"
#include "moc_githubpluginauthentication.cpp"
