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

#include "ddpclient.h"
#include "ruqola.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>


void process_test(QJsonDocument doc)
{
    qDebug() << "Callback test:" << doc;
    qDebug() << "End callback";
}

void login_callback(QJsonDocument doc)
{
    qDebug() << "LOGIN:" << doc;
    Ruqola::self()->setAuthToken(doc.object().value("token").toString());
    qDebug() << "End callback";
}

void DDPClient::resume_login_callback(const QJsonDocument &doc)
{
    qDebug() << "LOGIN:" << doc;
    Ruqola::self()->setAuthToken(doc.object().value("token").toString());
    qDebug() << "End callback";
}

void empty_callback(QJsonDocument doc)
{
    Q_UNUSED(doc);
}

DDPClient::DDPClient(const QString& url, QObject* parent)
 : QObject(parent),
  m_url(url),
  m_uid(1),
  m_loginJob(0),
  m_loginStatus(NotConnected),
  m_loginType(Password),
  m_connected(false),
  m_attemptedPasswordLogin(false),
  m_attemptedTokenLogin(false)
{
    m_webSocket.ignoreSslErrors();
    connect(&m_webSocket, &QWebSocket::connected, this, &DDPClient::onWSConnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &DDPClient::onTextMessageReceived);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &DDPClient::onWSclosed);
    connect(Ruqola::self(), &Ruqola::serverURLChanged, this, &DDPClient::onServerURLChange);    
    
    if (!url.isEmpty()) {
        m_webSocket.open(QUrl("wss://"+url+"/websocket"));
    }
    qDebug() << "Trying to connect to URL" << url;
    
}

DDPClient::~DDPClient()
{
    m_webSocket.close();
}

void DDPClient::onServerURLChange()
{
    if (Ruqola::self()->serverURL() != m_url || !m_webSocket.isValid()) {
        if (m_webSocket.isValid()) {
            m_webSocket.flush();
            m_webSocket.close();
        }
        m_url = Ruqola::self()->serverURL();
        m_webSocket.open(QUrl("wss://"+m_url+"/websocket"));
        connect(&m_webSocket, &QWebSocket::connected, this, &DDPClient::onWSConnected);
        qDebug() << "Reconnecting" << m_url;
    }
}

DDPClient::LoginStatus DDPClient::loginStatus() const
{
    return m_loginStatus;
}

void DDPClient::setLoginStatus(DDPClient::LoginStatus l)
{
    qDebug() << "Setting login status to" << l;
    m_loginStatus = l;
    emit loginStatusChanged();

    // reset flags
    if (l == LoginFailed) {
        m_attemptedPasswordLogin = false;
        m_attemptedTokenLogin = false;
    }
}


DDPClient::LoginType DDPClient::loginType() const
{
    return m_loginType;
}

void DDPClient::setLoginType(DDPClient::LoginType t)
{
    qDebug() << "Setting login type to" << t;
    m_loginType = t;
    emit loginTypeChanged();
}

bool DDPClient::isConnected() const
{
    return m_connected;
}

bool DDPClient::isLoggedIn() const
{
    return m_loginStatus == LoggedIn;
}

QString DDPClient::cachePath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
}

QQueue<QPair<QString,QJsonDocument>> DDPClient::messageQueue()
{
    return m_messageQueue;
}

unsigned int DDPClient::method(const QString& m, const QJsonDocument& params, DDPClient::MessageType messageType)
{
    return method(m, params, empty_callback, messageType);
}

unsigned int DDPClient::method(const QString& method, const QJsonDocument& params, std::function<void (QJsonDocument)> callback, DDPClient::MessageType messageType)
{
    QJsonObject json;
    json["msg"] = "method";
    json["method"] = method;
    json["id"] = QString::number(m_uid);

    if (params.isArray()){
        json["params"] = params.array();
    } else if (params.isObject()) {
        QJsonArray arr;
        arr.append(params.object());
        json["params"] = arr;
    }
        
    qint64 bytes = m_webSocket.sendTextMessage(QJsonDocument(json).toJson(QJsonDocument::Compact));
    if (bytes < json.length()) {
        qDebug() << "ERROR! I couldn't send all of my message. This is a bug! (try again)";
        qDebug() << m_webSocket.isValid() << m_webSocket.error() << m_webSocket.requestUrl();

        if (messageType==DDPClient::Persistent) {
            m_messageQueue.enqueue(qMakePair(method,params));
            Ruqola::self()->messageQueue()->processQueue();
        }
    } else {
        qDebug() << "Successfully sent " << json;
    }

    m_callbackHash[m_uid] = callback;

    m_uid++;
    return m_uid - 1 ;
}

void DDPClient::subscribe(const QString& collection, const QJsonArray& params)
{
    QJsonObject json;
    json["msg"] = "sub";
    json["id"] = QString::number(m_uid);
    json["name"] = collection;
    json["params"] = params;
    
    qint64 bytes = m_webSocket.sendTextMessage(QJsonDocument(json).toJson(QJsonDocument::Compact));
    if (bytes < json.length()) {
        qDebug() << "ERROR! I couldn't send all of my message. This is a bug! (try again)";
    }
    m_uid++;
}

void DDPClient::onTextMessageReceived(const QString &message)
{
    QJsonDocument response = QJsonDocument::fromJson(message.toUtf8());
    if (!response.isNull() && response.isObject()) {

        QJsonObject root = response.object();

        QString messageType = root.value("msg").toString();

        if (messageType == "updated") {

        } else if (messageType == "result") {

            unsigned id = root.value("id").toString().toInt();
            
            if (m_callbackHash.contains(id)) {
                    std::function<void (QJsonDocument)> callback = m_callbackHash.take(id);

                    callback( QJsonDocument(root.value("result").toObject()) );
            }
            emit result(id, QJsonDocument(root.value("result").toObject()));

            if (id == m_loginJob) {

                 if (root.value("error").toObject().value("error").toInt() == 403) {
                    qDebug() << "Wrong password or token expired";

                    login(); // Let's keep trying to log in
                 } else {
                    Ruqola::self()->setAuthToken(root.value("result").toObject().value("token").toString());
                    setLoginStatus(DDPClient::LoggedIn);
                 }
            }
        } else if (messageType == "connected") {
            qDebug() << "Connected";
            m_connected = true;
            emit connectedChanged();
            setLoginStatus(DDPClient::LoggingIn);
            //Ruqola::self()->authentication()->OAuthLogin();

            login(); // Try to resume auth token login
        } else if (messageType == "error") {
            qDebug() << "ERROR!!" << message;
        } else if (messageType == "ping") {
            qDebug() << "Ping - Pong";
            QJsonObject pong;
            pong["msg"] = "pong";
            m_webSocket.sendBinaryMessage(QJsonDocument(pong).toJson(QJsonDocument::Compact));
        } else if (messageType == "added"){
            qDebug() << "ADDING" <<root;
            emit added(root);
        } else if (messageType == "changed") {
            emit changed(root);
        } else if (messageType == "ready") {
            // do nothing
        } else {
            qDebug() << "received something unhandled:" << message;
        }
    }
}


void DDPClient::login()
{
    if (!Ruqola::self()->password().isEmpty()) {

        // If we have a password and we couldn't log in, let's stop here
        if (m_attemptedPasswordLogin) {
            setLoginStatus(LoginFailed);
            return;
        }
        m_attemptedPasswordLogin = true;
        QJsonObject user;
        user["username"] = Ruqola::self()->userName();
        QJsonObject json;
        json["password"] = Ruqola::self()->password();
        json["user"] = user;
        m_loginJob = method("login", QJsonDocument(json));
    } else if (!Ruqola::self()->authToken().isEmpty() && !m_attemptedTokenLogin) {
        m_attemptedPasswordLogin = true;
        QJsonObject json;
        json["resume"] = Ruqola::self()->authToken();
        m_loginJob = method("login", QJsonDocument(json));
    } else {
        setLoginStatus(LoginFailed);
    }
}

void DDPClient::onWSConnected()
{
    qDebug() << "Websocket connected at URL" << m_url;
    
    QJsonArray supportedVersions;
    supportedVersions.append("1");
    QJsonObject protocol;
    protocol["msg"] = "connect";
    protocol["version"] = "1";
    protocol["support"] = supportedVersions;
    QByteArray serialize = QJsonDocument(protocol).toJson(QJsonDocument::Compact);
    qint64 bytes = m_webSocket.sendTextMessage(serialize);
    if (bytes < serialize.length()) {
        qDebug() << "onWSConnected: ERROR! I couldn't send all of my message. This is a bug! (try again)";
    } else {
        qDebug() << "Successfully sent " << serialize;
    }
}

void DDPClient::onWSclosed()
{
    qDebug() << "WebSocket CLOSED" << m_webSocket.closeReason() << m_webSocket.error() << m_webSocket.closeCode();
    setLoginStatus(NotConnected);
}


