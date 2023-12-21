/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "githubauthenticationjob.h"
#include <QDesktopServices>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>

using namespace Qt::StringLiterals;
static constexpr auto authorizationUrl("https://github.com/login/oauth/authorize"_L1);
static constexpr auto accessTokenUrl("https://github.com/login/oauth/access_token"_L1);
static constexpr auto scope("identity read"_L1);

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
    mOAuth2->setAuthorizationUrl(QUrl(authorizationUrl));
    mOAuth2->setAccessTokenUrl(QUrl(accessTokenUrl));
    mOAuth2->setScope(scope);
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
