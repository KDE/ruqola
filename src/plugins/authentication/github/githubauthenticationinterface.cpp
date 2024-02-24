/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

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

PluginAuthenticationConfigureWidget *GitHubAuthenticationInterface::configureWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

void GitHubAuthenticationInterface::login()
{
    auto job = new GitHubAuthenticationJob(this);
    job->start();
    // connect(job, &GitHubAuthenticationJob::authenticated, this, [this]() {
    //     // TODO
    // });
}

#include "moc_githubauthenticationinterface.cpp"
