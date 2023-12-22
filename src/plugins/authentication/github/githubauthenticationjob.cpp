/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "githubauthenticationjob.h"
#include <QDesktopServices>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>

GitHubAuthenticationJob::GitHubAuthenticationJob(QObject *parent)
    : QObject{parent}
    , mOAuth2(new QOAuth2AuthorizationCodeFlow(this))
{
}

GitHubAuthenticationJob::~GitHubAuthenticationJob() = default;

void GitHubAuthenticationJob::start()
{
    auto replyHandler = new QOAuthHttpServerReplyHandler(1337, this);
    mOAuth2->setReplyHandler(replyHandler);
    mOAuth2->setAuthorizationUrl(QUrl(QStringLiteral("https://github.com/login/oauth/authorize")));
    mOAuth2->setAccessTokenUrl(QUrl(QStringLiteral("https://github.com/login/oauth/access_token")));
    mOAuth2->setScope(QStringLiteral("identity read"));
    mOAuth2->setClientIdentifier(QStringLiteral("clientIdentifier")); // TODO

    connect(mOAuth2, &QOAuth2AuthorizationCodeFlow::granted, this, &GitHubAuthenticationJob::slotAuthenticated);
    connect(mOAuth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, this, &QDesktopServices::openUrl);
    deleteLater();
}

void GitHubAuthenticationJob::slotAuthenticated()
{
    Q_EMIT authenticated();
    deleteLater();
}

#include "moc_githubauthenticationjob.cpp"
