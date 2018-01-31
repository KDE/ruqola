/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ruqola.h"
#include "authentication.h"
#include "ddpapi/ddpclient.h"
#include "ruqola_debug.h"
#include "rocketchataccount.h"

#include <QOAuth2AuthorizationCodeFlow>
#include <QFile>
#include <QJsonArray>
#include <QSettings>
#include <QUuid>

Authentication::Authentication()
{
    getDataFromJson();
}

void Authentication::getDataFromJson()
{
    QFile f(QStringLiteral(":/client_secret.json"));

    QString val;
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        val = QString::fromLatin1(f.readAll());
    } else {
        qCWarning(RUQOLA_LOG) << "Impossible to read client_secret.json";
        //TODO exit ?
    }

    QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject object = document.object();
    const auto settingsObject = object[QStringLiteral("web")].toObject();
    const QUrl authUri(settingsObject[QStringLiteral("auth_uri")].toString());
    const QUrl tokenUri(settingsObject[QStringLiteral("token_uri")].toString());
    const auto clientID = settingsObject[QStringLiteral("client_id")].toString();
    const auto clientSecret(settingsObject[QStringLiteral("client_secret")].toString());
    const auto redirectUrls = settingsObject[QStringLiteral("redirect_uris")].toArray();
    const QUrl redirectUrl(redirectUrls[0].toString());

/*
    QString clientID = QString("143580046552-s4rmnq5mg008u76id0d3rl63od985hc6.apps.googleusercontent.com");
    QString clientSecret = QString("nyVm19iOjjtldcCZJ-7003xg");
    QString redirectUrl = QString("http://localhost:8080/cb/_oauth/google?close");
*/

    QSettings s;
    s.setValue(QStringLiteral("clientID"), clientID);
    m_clientID = clientID;
    s.setValue(QStringLiteral("clientSecret"), clientSecret);
    m_clientSecret = clientSecret;
    s.setValue(QStringLiteral("redirectUrl"), redirectUrl);
}

void Authentication::OAuthLogin()
{
    QJsonObject auth;
    QJsonObject authKeys;
    authKeys[QStringLiteral("credentialToken")] = m_clientID;
    authKeys[QStringLiteral("credentialSecret")] = m_clientSecret;

    auth[QStringLiteral("oauth")] = authKeys;
    qCDebug(RUQOLA_LOG) << "-------------------------";
    qCDebug(RUQOLA_LOG) << "-------------------------";
    qCDebug(RUQOLA_LOG) << "OAuth Json" << auth;
    Ruqola::self()->rocketChatAccount()->ddp()->method(QStringLiteral("login"), QJsonDocument(auth));

    QJsonArray requestPermissions;
    requestPermissions.append(QStringLiteral("email"));

    QUuid state;
    state = state.createUuid();
    QSettings s;
    s.setValue(QStringLiteral("stateRandomNumber"), state);

    QJsonObject loginUrlParameters;
    loginUrlParameters[QStringLiteral("client_id")] = m_clientID;
    loginUrlParameters[QStringLiteral("response_type")] = QStringLiteral("code");
    loginUrlParameters[QStringLiteral("scope")] = QStringLiteral("openID profile email");
    loginUrlParameters[QStringLiteral("state")] = state.toString();

    QJsonObject json;
    json[QStringLiteral("requestPermissions")] = requestPermissions;
    json[QStringLiteral("requestOfflineToken")] = true;
    json[QStringLiteral("loginUrlParameters")] = loginUrlParameters;
    json[QStringLiteral("loginHint")] = s.value(QStringLiteral("username")).toString();
    json[QStringLiteral("loginStyle")] = QStringLiteral("redirect");
    json[QStringLiteral("redirectUrl")] = s.value(QStringLiteral("redirectUrl")).toString();

//    qCDebug(RUQOLA_LOG) << "OAuth Json" << json;
//    Ruqola::self()->ddp()->method("login", QJsonDocument(json));
}
