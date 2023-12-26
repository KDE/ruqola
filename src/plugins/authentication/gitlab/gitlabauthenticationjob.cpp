/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gitlabauthenticationjob.h"
#include "gitlabauthenticationplugin_debug.h"
#include <QDesktopServices>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QTimer>

GitLabAuthenticationJob::GitLabAuthenticationJob(QObject *parent)
    : QObject{parent}
    , mOAuth2(new QOAuth2AuthorizationCodeFlow(this))
{
}

GitLabAuthenticationJob::~GitLabAuthenticationJob() = default;

void GitLabAuthenticationJob::doRequest()
{
    // TODO
    // TODO use https://github.com/login/device/code

    deleteLater();
}

GitLabAuthenticationJob::GitLabInfo GitLabAuthenticationJob::gitLabInfo() const
{
    return mGitLabInfo;
}

void GitLabAuthenticationJob::setGitLabInfo(const GitLabInfo &newGitLabInfo)
{
    mGitLabInfo = newGitLabInfo;
}

void GitLabAuthenticationJob::start()
{
    if (!mGitLabInfo.isValid()) {
        qCWarning(RUQOLA_GITLABAUTHENTICATION_PLUGIN_LOG) << "Lab info is invalid";
        deleteLater();
        return;
    }
    auto replyHandler = new QOAuthHttpServerReplyHandler(11450, mOAuth2);
    mOAuth2->setClientIdentifier(mGitLabInfo.clientId);
    mOAuth2->setReplyHandler(replyHandler);
    mOAuth2->setAuthorizationUrl(QUrl(QStringLiteral("https://gitlab.com/login/oauth/authorize")));
    mOAuth2->setAccessTokenUrl(QUrl(QStringLiteral("https://gitlab.com/login/oauth/access_token")));
    mOAuth2->setScope(QStringLiteral("openid"));

    mOAuth2->setToken(mGitLabInfo.token);
    mOAuth2->setRefreshToken(mGitLabInfo.refreshToken);

    QObject::connect(mOAuth2, &QOAuth2AuthorizationCodeFlow::statusChanged, [this](QAbstractOAuth::Status status) {
        qCDebug(RUQOLA_GITLABAUTHENTICATION_PLUGIN_LOG)
            << (int)status << mOAuth2->token() << mOAuth2->refreshToken() << mOAuth2->expirationAt() << mOAuth2->extraTokens();
        if (status == QAbstractOAuth::Status::Granted) {
            // TODO store info in qtkeychain
            qCDebug(RUQOLA_GITLABAUTHENTICATION_PLUGIN_LOG) << "authorization granted";
            doRequest();
            QTimer::singleShot(std::max<qint64>(5 * 60 * 1000, QDateTime::currentDateTime().secsTo(mOAuth2->expirationAt()) * 800),
                               mOAuth2,
                               &QOAuth2AuthorizationCodeFlow::refreshAccessToken);
        }
    });
    QObject::connect(mOAuth2, &QOAuth2AuthorizationCodeFlow::authorizationCallbackReceived, [](const QVariantMap &m) {
        qCDebug(RUQOLA_GITLABAUTHENTICATION_PLUGIN_LOG) << "auth callback received" << m;
    });
    QObject::connect(mOAuth2, &QOAuth2AuthorizationCodeFlow::error, [](const QString &err, const QString &desc) {
        qCDebug(RUQOLA_GITLABAUTHENTICATION_PLUGIN_LOG) << "error" << err << desc;
    });
    QObject::connect(mOAuth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);

    if (mOAuth2->refreshToken().isEmpty()) {
        mOAuth2->grant();
    } else if (!mOAuth2->expirationAt().isValid() || mOAuth2->expirationAt() < QDateTime::currentDateTimeUtc()) {
        mOAuth2->refreshAccessToken();
    }
}

bool GitLabAuthenticationJob::GitLabInfo::isValid() const
{
    return !url.isEmpty() && !clientId.isEmpty();
}
#include "moc_gitlabauthenticationjob.cpp"
