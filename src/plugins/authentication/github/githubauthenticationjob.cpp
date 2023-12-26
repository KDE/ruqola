/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "githubauthenticationjob.h"
#include "githubauthenticationplugin_debug.h"
#include "githubauthenticationsettings.h"
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
#if 0
    QString clientId = "CLIENT ID FROM API CREDENTIALS";
    QString authUri = "https://accounts.google.com/o/oauth2/auth";
    QString tokenUri = "https://accounts.google.com/o/oauth2/token";
    QStringList redirectUris;
    redirectUris << "http://127.0.0.1:54321/"; //ensure this is included in the API credentials under authorized URI
    QString redirectUri = redirectUris[0];
    QString clientSecret = "YOUR CLIENT SECRET FROM API CREDENTIALS";
    auto google = new QOAuth2AuthorizationCodeFlow(this);
    google->setScope("email");
    google->setAuthorizationUrl(authUri);
    google->setClientIdentifier(clientId);
    google->setAccessTokenUrl(tokenUri);
    google->setClientIdentifierSharedKey(clientSecret);

    // Use the same port number as above
    auto replyHandler = new QOAuthHttpServerReplyHandler(54321, this);
    google->setReplyHandler(replyHandler);

    // Set up the function to modify AND REPLACE the parameters
    google->setModifyParametersFunction([](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant>* parameters) {
        if (stage == QAbstractOAuth::Stage::RequestingAccessToken) {
            auto encodedCode = parameters->value("code").toByteArray();
            parameters->replace("code", QUrl::fromPercentEncoding(encodedCode));
        }
    });

    // Connect the signals to retrieve the tokens, if you want to see them
    connect(google, &QOAuth2AuthorizationCodeFlow::tokenChanged, [=](const QString &token)
    {
        qDebug() << "Token changed:" << token;

    });
    connect(replyHandler, &QOAuthHttpServerReplyHandler::tokensReceived, [](const QVariantMap &tokens)
    {
        qDebug() << "Tokens received:" << tokens;
    });

    connect(google, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);

    // Once you are granted access, make a request via the Google API
    connect(google, &QOAuth2AuthorizationCodeFlow::granted, [=]
    {
        qDebug() << "in the granted block now";
        // This block is run once you have logged into the browser successfully

        auto rep = google->get(QUrl("https://www.googleapis.com/oauth2/v1/userinfo?alt=json"));
        QEventLoop loop;
        connect(rep, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        QString currentByteArray = rep->readAll();
        qDebug() << "network reply google api connect step get info" << currentByteArray;

    });

    google->grant();

    0

#endif

    auto replyHandler = new QOAuthHttpServerReplyHandler(11450, mOAuth2);
    mOAuth2->setClientIdentifier(mGitHubInfo.clientId);
    mOAuth2->setReplyHandler(replyHandler);
    mOAuth2->setAuthorizationUrl(GitHubAuthenticationSettings::authorizeUrl());
    mOAuth2->setAccessTokenUrl(GitHubAuthenticationSettings::accessTokenUrl());
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
