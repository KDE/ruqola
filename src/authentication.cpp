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
#include "ddpclient.h"

#include <QOAuth2AuthorizationCodeFlow>
#include <QFile>

Authentication::Authentication()
{
    getDataFromJson();
}

void Authentication::getDataFromJson(){

    QDir cacheDir(":/src");
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
    const QUrl tokenUri(settingsObject["token_uri"].toString());
    const auto clientID = settingsObject["client_id"].toString();
    const auto clientSecret(settingsObject["client_secret"].toString());
    const auto redirectUrls = settingsObject["redirect_uris"].toArray();
    const QUrl redirectUrl(redirectUrls[0].toString());

/*
    QString clientID = QString("143580046552-s4rmnq5mg008u76id0d3rl63od985hc6.apps.googleusercontent.com");
    QString clientSecret = QString("nyVm19iOjjtldcCZJ-7003xg");
    QString redirectUrl = QString("http://localhost:8080/cb/_oauth/google?close");
*/

    QSettings s;
    s.setValue("clientID", clientID);
    m_clientID = clientID;
    s.setValue("clientSecret", clientSecret);
    m_clientSecret = clientSecret;
    s.setValue("redirectUrl", redirectUrl);
}


void Authentication::OAuthLogin() {

    QJsonObject auth;
    QJsonObject authKeys;
    authKeys["credentialToken"] = m_clientID;
    authKeys["credentialSecret"] = m_clientSecret;

    auth["oauth"] = authKeys;
    qDebug() << "-------------------------";
    qDebug() << "-------------------------";
    qDebug() << "OAuth Json" << auth;
    Ruqola::self()->ddp()->method("login", QJsonDocument(auth));

    QJsonArray requestPermissions;
    requestPermissions.append("email");

    QUuid state;
    state = state.createUuid();
    QSettings s;
    s.setValue("stateRandomNumber", state);

    QJsonObject loginUrlParameters;
    loginUrlParameters["client_id"] = m_clientID;
    loginUrlParameters["response_type"] = QString("code");
    loginUrlParameters["scope"] = QString("openID profile email");
    loginUrlParameters["state"] = state.toString();

    QJsonObject json;
    json["requestPermissions"] = requestPermissions;
    json["requestOfflineToken"] = true;
    json["loginUrlParameters"] = loginUrlParameters;
    json["loginHint"] = s.value("username").toString();
    json["loginStyle"] = QString("redirect");
    json["redirectUrl"] = s.value("redirectUrl").toString();

//    qDebug() << "OAuth Json" << json;
//    Ruqola::self()->ddp()->method("login", QJsonDocument(json));

}


//#include "authentication.moc"
