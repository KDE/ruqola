/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "githubauthenticationjob.h"
#include "githubauthenticationplugin_debug.h"
#include <QDesktopServices>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QTimer>

GitHubAuthenticationJob::GitHubAuthenticationJob(QObject *parent)
    : QObject{parent}
    , mOAuth2(new QOAuth2AuthorizationCodeFlow(this))
{
}

GitHubAuthenticationJob::~GitHubAuthenticationJob() = default;

void GitHubAuthenticationJob::doRequest()
{
    // TODO

    deleteLater();
}

GitHubAuthenticationJob::GitHubInfo GitHubAuthenticationJob::gitHubInfo() const
{
    return mGitHubInfo;
}

void GitHubAuthenticationJob::setGitHubInfo(const GitHubInfo &newGitHubInfo)
{
    mGitHubInfo = newGitHubInfo;
}

void GitHubAuthenticationJob::start()
{
    if (!mGitHubInfo.isValid()) {
        qCWarning(RUQOLA_GITHUBAUTHENTICATION_PLUGIN_LOG) << "Lab info is invalid";
        deleteLater();
        return;
    }
    auto replyHandler = new QOAuthHttpServerReplyHandler(11450, mOAuth2);
    mOAuth2->setClientIdentifier(mGitHubInfo.clientId);
    mOAuth2->setReplyHandler(replyHandler);
    mOAuth2->setAuthorizationUrl(QUrl(QStringLiteral("https://github.com/login/oauth/authorize")));
    mOAuth2->setAccessTokenUrl(QUrl(QStringLiteral("https://github.com/login/oauth/access_token")));
    mOAuth2->setScope(QStringLiteral("openid"));
    mOAuth2->setToken(mGitHubInfo.token);
    mOAuth2->setRefreshToken(mGitHubInfo.refreshToken);

    QObject::connect(mOAuth2, &QOAuth2AuthorizationCodeFlow::statusChanged, [this](QAbstractOAuth::Status status) {
        qCDebug(RUQOLA_GITHUBAUTHENTICATION_PLUGIN_LOG)
            << (int)status << mOAuth2->token() << mOAuth2->refreshToken() << mOAuth2->expirationAt() << mOAuth2->extraTokens();
        if (status == QAbstractOAuth::Status::Granted) {
            // TODO store info in qtkeychain
            qCDebug(RUQOLA_GITHUBAUTHENTICATION_PLUGIN_LOG) << "authorization granted";
            doRequest();
            QTimer::singleShot(std::max<qint64>(5 * 60 * 1000, QDateTime::currentDateTime().secsTo(mOAuth2->expirationAt()) * 800),
                               mOAuth2,
                               &QOAuth2AuthorizationCodeFlow::refreshAccessToken);
        }
    });
    QObject::connect(mOAuth2, &QOAuth2AuthorizationCodeFlow::authorizationCallbackReceived, [](const QVariantMap &m) {
        qCDebug(RUQOLA_GITHUBAUTHENTICATION_PLUGIN_LOG) << "auth callback received" << m;
    });
    QObject::connect(mOAuth2, &QOAuth2AuthorizationCodeFlow::error, [](const QString &err, const QString &desc) {
        qCDebug(RUQOLA_GITHUBAUTHENTICATION_PLUGIN_LOG) << "error" << err << desc;
    });
    QObject::connect(mOAuth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);

    if (mOAuth2->refreshToken().isEmpty()) {
        mOAuth2->grant();
    } else if (!mOAuth2->expirationAt().isValid() || mOAuth2->expirationAt() < QDateTime::currentDateTimeUtc()) {
        mOAuth2->refreshAccessToken();
    }
}

bool GitHubAuthenticationJob::GitHubInfo::isValid() const
{
    return !url.isEmpty() && !clientId.isEmpty();
}

#include "moc_githubauthenticationjob.cpp"
