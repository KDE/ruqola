/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "githubauthenticationinterface.h"
#include "githubauthenticationjob.h"
#include "rocketchataccount.h"

GitHubAuthenticationInterface::GitHubAuthenticationInterface(QObject *parent)
    : PluginAuthenticationInterface(parent)
{
}

GitHubAuthenticationInterface::~GitHubAuthenticationInterface() = default;

PluginAuthenticationConfigureWidget *GitHubAuthenticationInterface::configureWidget([[maybe_unused]] QWidget *parent)
{
    return nullptr;
}

bool GitHubAuthenticationInterface::login()
{
    auto job = new GitHubAuthenticationJob(this);
    job->start();
    // connect(job, &GitHubAuthenticationJob::authenticated, this, [this]() {
    //     // TODO
    // });
    // Async we need to find another method for it but not implement yet => no problem
    return true;
}

#include "moc_githubauthenticationinterface.cpp"
