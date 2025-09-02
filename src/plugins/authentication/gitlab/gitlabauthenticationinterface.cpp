/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gitlabauthenticationinterface.h"
#include "gitlabauthenticationjob.h"
#include "rocketchataccount.h"

GitLabAuthenticationInterface::GitLabAuthenticationInterface(QObject *parent)
    : PluginAuthenticationInterface(parent)
{
}

GitLabAuthenticationInterface::~GitLabAuthenticationInterface() = default;

bool GitLabAuthenticationInterface::login()
{
    // Login if we don't have valid token.
    auto job = new GitLabAuthenticationJob(this);
    job->start();
    // connect(job, &GitLabAuthenticationJob::authenticated, this, [this]() {
    //     // TODO
    // });
    // Async we need to find another method for it but not implement yet => no problem
    return true;
}

PluginAuthenticationConfigureWidget *GitLabAuthenticationInterface::configureWidget([[maybe_unused]] QWidget *parent)
{
    return nullptr;
}

#include "moc_gitlabauthenticationinterface.cpp"
