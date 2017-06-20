#include "ruqola.h"
#include "authentication.h"


#include <QAbstractOAuthReplyHandler>
#include <QOAuth2AuthorizationCodeFlow>
#include <QtCore>
#include <QFile>

Authentication::Authentication(){

    m_google->setScope("email");
//    connect(m_google, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, this, &QDesktopServices::openUrl);
//    connect(&m_google, &QOAuth2AuthorizationCodeFlow::granted, this, &Authentication::onGranted());


}



void Authentication::OAuthLogin() {
    QJsonObject authKeys;
    authKeys["credentialToken"] = m_client_id;
    authKeys["credentialSecret"] = m_client_secret;

    Ruqola::self()->ddp()->method("login", QJsonDocument(authKeys));

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
    const auto redirectUris = settingsObject["redirect_uris"].toArray();
    const QUrl redirectUri(redirectUris[0].toString());
    const auto port = static_cast<quint16>(redirectUri.port());

    m_google->setAuthorizationUrl(authUri);
    m_google->setClientIdentifier(clientId);
    m_google->setAccessTokenUrl(tokenUri);
    m_google->setClientIdentifierSharedKey(clientSecret);

    auto replyHandler = new QOAuthHttpServerReplyHandler(port, this);
    m_google->setReplyHandler(replyHandler);

    // If user grants the permissions, client receives a QOAuth2AuthorizationCodeFlow::granted signal
    // and can then start sending authorized requests.
    m_google->grant();

}

void Authentication::onGranted()
{
    m_authGranted = true;
}

void Authentication::sendApiRequest()
{
   if(!m_authGranted) return;

   //Try sending a request using https://www.googleapis.com/plus/v1/people/me
   auto reply = m_google->get(QUrl("https://www.googleapis.com/plus/v1/people/me"));
}



/*
 * Send a request to Google's OAuth 2.0 server
 *
https://accounts.google.com/o/oauth2/v2/auth?
 scope=email%20profile&
 response_type=code&
 state=security_token%3D138r5719ru3e1%26url%3Dhttps://oauth2.example.com/token&
 redirect_uri=http://127.0.0.1:9004&
 client_id=client_id

*/



/*
 * Handle the OAuth 2.0 server response
 * Exchange authorization code for refresh and access tokens
 *
 *
 *
 * REQUEST
 * ------------------------
 *  POST /oauth2/v4/token HTTP/1.1
    Host: www.googleapis.com
    Content-Type: application/x-www-form-urlencoded

    code=4/P7q7W91a-oMsCeLvIaQm6bTrgtp7&
    client_id=your_client_id&
    client_secret=your_client_secret&
    redirect_uri=https://oauth2.example.com/code&
    grant_type=authorization_code


Google responds to this request by returning a JSON object that contains a short-lived access token and a refresh token.

  *RESPONSE
  * ----------------------------
  * The following snippet shows a sample response:

    {
      "access_token":"1/fFAGRNJru1FTz70BzhT3Zg",
      "expires_in":3920,
      "token_type":"Bearer",
      "refresh_token":"1/xEoDL4iW3cxlI7yDbSRFYNG01kVKM2C-259HOF2aQbI"
    }


 *CALLING GOOGLE APIs
 * -------------------------------
    GET https://www.googleapis.com/drive/v2/files?access_token=<access_token>


*/
