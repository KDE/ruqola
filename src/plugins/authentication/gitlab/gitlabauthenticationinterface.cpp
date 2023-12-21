/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

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
    auto job = new GitLabAuthenticationJob(this);
    job->start();
    connect(job, &GitLabAuthenticationJob::authenticated, this, [this]() {
        // TODO
    });
}

#include "moc_gitlabauthenticationinterface.cpp"
