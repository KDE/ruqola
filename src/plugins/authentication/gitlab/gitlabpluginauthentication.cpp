/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gitlabpluginauthentication.h"
#include "gitlabauthenticationinterface.h"
#include <KLocalizedString>
#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(GitLabPluginAuthentication, "ruqola_gitlabauthenticationplugin.json")

GitLabPluginAuthentication::GitLabPluginAuthentication(QObject *parent, const QVariantList &)
    : PluginAuthentication(parent)
{
}

GitLabPluginAuthentication::~GitLabPluginAuthentication() = default;

PluginAuthenticationInterface *GitLabPluginAuthentication::createInterface(QObject *parent)
{
    return new GitLabAuthenticationInterface(parent);
}

AuthenticationManager::AuthMethodType GitLabPluginAuthentication::type() const
{
    return AuthenticationManager::AuthMethodType::GitLab;
}

QString GitLabPluginAuthentication::name() const
{
    return i18n("GitLab");
}

QString GitLabPluginAuthentication::iconName() const
{
    return QStringLiteral("dialog-password");
}

#include "gitlabpluginauthentication.moc"
#include "moc_gitlabpluginauthentication.cpp"
