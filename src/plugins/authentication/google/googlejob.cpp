/*
 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2018 Veluri Mithun <velurimithun38@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 *
 *
 */

#include "googlejob.h"
using namespace Qt::Literals::StringLiterals;

#include "googleauthenticationplugin_debug.h"
#include "ruqola_debug.h"

#include <QDesktopServices>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <o2/o0globals.h>
#include <o2/o0settingsstore.h>
#include <o2/o2google.h>

GoogleJob::GoogleJob(QObject *parent)
    : QObject(parent)
{
    mO2Google = new O2Google(this);

    getDataFromJson();

    mO2Google->setClientId(m_clientID);
    mO2Google->setClientSecret(m_clientSecret);
    mO2Google->setLocalPort(8888); // it is from redirect url(http://127.0.0.1:8888/)
    mO2Google->setRequestUrl(m_authUri); // Use the desktop login UI
    mO2Google->setScope(u"email"_s);

    // Create a store object for writing the received tokens
    O0SettingsStore *store = new O0SettingsStore(QLatin1StringView(O2_ENCRYPTION_KEY), this);
    store->setGroupKey(u"Google"_s);
    mO2Google->setStore(store);

    connect(mO2Google, &O2Google::linkedChanged, this, &GoogleJob::onLinkedChanged);
    connect(mO2Google, &O2Google::linkingFailed, this, [this]() {
        Q_EMIT linkingFailed();
    });
    connect(mO2Google, &O2Google::linkingSucceeded, this, &GoogleJob::onLinkingSucceeded);
    connect(mO2Google, &O2Google::openBrowser, this, &GoogleJob::onOpenBrowser);
    connect(mO2Google, &O2Google::closeBrowser, this, &GoogleJob::onCloseBrowser);
    connect(mO2Google, &O2Google::linkingSucceeded, this, &GoogleJob::OAuthLoginMethodParameter);
}

GoogleJob::~GoogleJob()
{
}

void GoogleJob::getDataFromJson()
{
    QFile f(u":/client_secret.json"_s);

    QString val;
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        val = QString::fromLatin1(f.readAll());
    } else {
        qCWarning(RUQOLA_GOOGLEAUTHENTICATION_PLUGIN_LOG) << "Impossible to read client_secret.json";
        // TODO exit ?
        return;
    }

    //******github*******
    // 38a607244195a0d7af8 > clientID
    // bb617841568d7c1e0c0888f292cf69b7b11d327e3 > clientSecret
    // https://github.com/login/oauth/authorize
    // https://github.com/login/oauth/access_token
    const QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());
    const QJsonObject object = document.object();
    const auto settingsObject = object[u"web"_s].toObject();
    const auto authUri(settingsObject[u"auth_uri"_s].toString());
    const auto clientID = settingsObject[u"client_id"_s].toString();
    const auto clientSecret(settingsObject[u"client_secret"_s].toString());

    m_clientID = clientID;
    m_clientSecret = clientSecret;
    m_authUri = authUri;
    m_tokenUri = u"https://accounts.google.com/o/oauth2/token"_s;
}

void GoogleJob::doOAuth(O2::GrantFlow grantFlowType)
{
    qCDebug(RUQOLA_GOOGLEAUTHENTICATION_PLUGIN_LOG) << "Starting OAuth 2 with grant flow type: Authorization Grant Flow...";
    mO2Google->setGrantFlow(grantFlowType);
    mO2Google->unlink();

    // TODO: refresh the token if it is expired(not valid)
    validateToken();
    if (m_isValidToken) {
        OAuthLoginMethodParameter();
    } else {
        mO2Google->link();
    }
}

// currently not used
void GoogleJob::validateToken()
{
    if (!mO2Google->linked()) {
        qCWarning(RUQOLA_GOOGLEAUTHENTICATION_PLUGIN_LOG) << "ERROR: Application is not linked!";
        Q_EMIT linkingFailed(QString());
        return;
    }

    const QString accessToken = mO2Google->token();
    QString debugUrlStr = QString(m_tokenUri).arg(accessToken);
    QNetworkRequest request = QNetworkRequest(QUrl(debugUrlStr));
    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    QNetworkReply *reply = mgr->get(request);
    connect(reply, &QNetworkReply::finished, this, &GoogleJob::onFinished);
    qCDebug(RUQOLA_GOOGLEAUTHENTICATION_PLUGIN_LOG) << u"Validating user token. Please wait..."_s;
}

void GoogleJob::onOpenBrowser(const QUrl &url)
{
    QDesktopServices::openUrl(url);
}

void GoogleJob::onCloseBrowser()
{
    // TODO: close the browser
}

void GoogleJob::onLinkedChanged()
{
    qCDebug(RUQOLA_GOOGLEAUTHENTICATION_PLUGIN_LOG) << u"Link changed!"_s;
}

void GoogleJob::onLinkingSucceeded()
{
    O2Google *o1t = qobject_cast<O2Google *>(sender());
    if (!o1t || !o1t->linked()) {
        return;
    }
    m_accessToken = o1t->token();
    const QVariantMap extraTokens = o1t->extraTokens();
    if (!extraTokens.isEmpty()) {
        Q_EMIT extraTokensReady(extraTokens);
        qCDebug(RUQOLA_GOOGLEAUTHENTICATION_PLUGIN_LOG) << u"Extra tokens in response:"_s;
        foreach (const QString &key, extraTokens.keys()) {
            qCDebug(RUQOLA_GOOGLEAUTHENTICATION_PLUGIN_LOG) << key << u":"_s << (extraTokens.value(key).toString().left(3) + u"..."_s);
        }
    }
}

// currently not used
void GoogleJob::onFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) {
        qCWarning(RUQOLA_GOOGLEAUTHENTICATION_PLUGIN_LOG) << "NULL reply!";
        Q_EMIT linkingFailed(QString());
        return;
    }

    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        qCWarning(RUQOLA_GOOGLEAUTHENTICATION_PLUGIN_LOG) << u"Reply error:"_s << reply->error();
        qCWarning(RUQOLA_GOOGLEAUTHENTICATION_PLUGIN_LOG) << u"Reason:"_s << reply->errorString();
        Q_EMIT linkingFailed(QString());
        return;
    }

    const QByteArray replyData = reply->readAll();
    bool valid = !replyData.contains("error");
    if (valid) {
        qCDebug(RUQOLA_GOOGLEAUTHENTICATION_PLUGIN_LOG) << u"Token is valid"_s;
        Q_EMIT linkingSucceeded();
        m_isValidToken = true;
    } else {
        qCDebug(RUQOLA_GOOGLEAUTHENTICATION_PLUGIN_LOG) << u"Token is invalid"_s;
        // TODO
        Q_EMIT linkingFailed(QString());
    }
}

void GoogleJob::OAuthLoginMethodParameter()
{
    Q_EMIT loginMethodCalled(m_accessToken, m_clientSecret);
}

#include "moc_googlejob.cpp"
