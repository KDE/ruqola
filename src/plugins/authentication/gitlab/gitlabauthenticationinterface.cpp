/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

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

void GitLabAuthenticationInterface::login()
{
    // Login if we don't have valid token.
    auto job = new GitLabAuthenticationJob(this);
    job->start();
    // connect(job, &GitLabAuthenticationJob::authenticated, this, [this]() {
    //     // TODO
    // });
}

#include "moc_gitlabauthenticationinterface.cpp"
