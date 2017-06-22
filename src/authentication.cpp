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

    QJsonArray requestPermissions;
    requestPermissions.append("email");

    bool requestOfflineToken = true;

    QString scope = QString("%1 %2 %3").arg("openID").arg("profile").arg("email");

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

    Ruqola::self()->ddp()->method("login", QJsonDocument(json));

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
