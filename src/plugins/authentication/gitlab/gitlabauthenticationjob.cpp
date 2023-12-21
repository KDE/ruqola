/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gitlabauthenticationjob.h"
#include <QDesktopServices>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>

using namespace Qt::StringLiterals;
static constexpr auto authorizationUrl("https://gitlab.com/login/oauth/authorize"_L1);
static constexpr auto accessTokenUrl("https://gitlab.com/login/oauth/access_token"_L1);
static constexpr auto scope("openid"_L1);

GitLabAuthenticationJob::GitLabAuthenticationJob(QObject *parent)
    : QObject{parent}
    , mOAuth2(new QOAuth2AuthorizationCodeFlow(this))
{
}

GitLabAuthenticationJob::~GitLabAuthenticationJob() = default;

void GitLabAuthenticationJob::start()
{
    auto replyHandler = new QOAuthHttpServerReplyHandler(1337, this);
    mOAuth2->setReplyHandler(replyHandler);
    mOAuth2->setAuthorizationUrl(QUrl(authorizationUrl));
    mOAuth2->setAccessTokenUrl(QUrl(accessTokenUrl));
    mOAuth2->setScope(scope);
    mOAuth2->setClientIdentifier(QStringLiteral("clientIdentifier")); // TODO

    connect(mOAuth2, &QOAuth2AuthorizationCodeFlow::granted, this, &GitLabAuthenticationJob::slotAuthenticated);
    connect(mOAuth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, this, &QDesktopServices::openUrl);
    deleteLater();
}

void GitLabAuthenticationJob::slotAuthenticated()
{
    Q_EMIT authenticated();
    deleteLater();
}

#include "moc_gitlabauthenticationjob.cpp"
