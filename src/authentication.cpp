/*
 * <one line to give the program's name and a brief idea of what it does.>
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

#include <QOAuth2AuthorizationCodeFlow>
#include <QFile>

Authentication::Authentication()
{
    getDataFromJson();
}

void Authentication::OAuthLogin() {
    QJsonObject authKeys;
    authKeys["credentialToken"] = m_client_id;
    authKeys["credentialSecret"] = m_client_secret;

    Ruqola::self()->ddp()->method("login", QJsonDocument(authKeys));

    QJsonArray requestPermissions;
    requestPermissions.append("email");

    bool requestOfflineToken = true;

    QString scope = QString("openID profile email");

    QSettings s;
    s.setValue("stateHexNumber", QString("{67C8770B-44F1-410A-AB9A-F9B5446F13EE}"));
    QUuid state(s.value("stateHexNumber").toString());

    QJsonObject loginUrlParameters;
    loginUrlParameters["client_id"] = m_client_id;
    loginUrlParameters["response_type"] = QString("code");
    loginUrlParameters["scope"] = scope;
    loginUrlParameters["state"] = state.toString();

    QString username = s.value("username").toString();
    QString loginHint = username;

    QString loginStyle = QString("redirect");
    QString redirectUrl = s.value("redirectUrl").toString();

    QJsonObject json;
    json["requestPermissions"] = requestPermissions;
    json["requestOfflineToken"] = requestOfflineToken;
    json["loginUrlParameters"] = loginUrlParameters;
    json["loginHint"] = loginHint;
    json["loginStyle"] = loginStyle;
    json["redirectUrl"] = redirectUrl;

//    Ruqola::self()->ddp()->method("login", QJsonDocument(json));

}


void Authentication::getDataFromJson(){

    QDir cacheDir(":/src/client_secret.json");
    if (!cacheDir.exists(cacheDir.path())) {
        cacheDir.mkpath(cacheDir.path());
    }

    QFile f(cacheDir.absoluteFilePath("client_secret.json"));

    QString val;
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            val = f.readAll();
    }

    QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject object = document.object();
    const auto settingsObject = object["web"].toObject();
    const QUrl authUri(settingsObject["auth_uri"].toString());
    const auto clientId = settingsObject["client_id"].toString();
    const QUrl tokenUri(settingsObject["token_uri"].toString());
    const auto clientSecret(settingsObject["client_secret"].toString());
    const auto redirectUrls = settingsObject["redirect_uris"].toArray();
    const QUrl redirectUrl(redirectUrls[0].toString());


    QSettings s;
    s.setValue("clientID", clientId);
    m_client_id = clientId;
    s.setValue("clientSecret", clientSecret);
    m_client_secret = clientSecret;
    s.setValue("redirectUrl", redirectUrl);
}

//#include "authentication.moc"
