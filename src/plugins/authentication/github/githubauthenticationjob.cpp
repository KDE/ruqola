/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "githubauthenticationjob.h"
#include <QOAuth2AuthorizationCodeFlow>

GitHubAuthenticationJob::GitHubAuthenticationJob(QObject *parent)
    : QObject{parent}
{
}

GitHubAuthenticationJob::~GitHubAuthenticationJob() = default;

void GitHubAuthenticationJob::start()
{
    // TODO
}
