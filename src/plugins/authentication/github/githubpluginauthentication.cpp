/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "githubpluginauthentication.h"
using namespace Qt::Literals::StringLiterals;

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

AuthenticationManager::AuthMethodType GitHubPluginAuthentication::authenticationType() const
{
    return AuthenticationManager::AuthMethodType::GitHub;
}

QString GitHubPluginAuthentication::name() const
{
    return i18n("GitHub");
}

QString GitHubPluginAuthentication::iconName() const
{
    return u"dialog-password"_s;
}

#include "githubpluginauthentication.moc"
#include "moc_githubpluginauthentication.cpp"
