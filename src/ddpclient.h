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

#ifndef DDPCLIENT_H
#define DDPCLIENT_H

#include <functional>
#include <QtCore>
#include <QWebSocket>


class QJsonObject;
class QJsonDocument;
class QUrl;
class QWebSocket;

class DDPClient : public QObject
{
    Q_OBJECT
public:
    enum LoginStatus {
        NotConnected,
        LoggingIn,
        LoggedIn,
        LoginFailed,
        LoggedOut
    };
    Q_ENUM(LoginStatus)

    enum MessageType {
        Persistent,
        Ephemeral
    };

    enum LoginType {
        Password,
        Google
    };

    DDPClient(const QString &url = QString(), QObject *parent = 0);
    ~DDPClient();

    /**
    * @brief Call a method with name @param method and parameters @param params and @param messageType with an empty callback
    *
    * @param method The name of the method to call Rocket.Chat API for
    * @param params The parameters
    * @param messageType The type of message
    * @return unsigned int, the ID of the called method
    */
    unsigned method(const QString &method, const QJsonDocument &params, DDPClient::MessageType messageType = DDPClient::Ephemeral);

    /**
    * @brief Send message over network
    *
    * @param method The name of the method to call Rocket.Chat API for
    * @param params The parameters
    * @param callback The pointer to callback function
    * @param messageType The type of message
    * @return unsigned int, the ID of the called method
    */
    unsigned method(const QString &method, const QJsonDocument &params, std::function<void (QJsonDocument)> callback, DDPClient::MessageType messageType = DDPClient::Ephemeral);


    /**
    * @brief Subscribes to a collection with name @param collection and parameters @param params
    *
    * @param collection The name of the collection
    * @param params The parameters
    */
    void subscribe(const QString &collection, const QJsonArray &params);

    /**
    * @brief Calls method to log in the user with valid username and password
    */
    Q_INVOKABLE void login();

    /**
    * @brief Closes the websocket connection
    */
    void logOut();

    /**
    * @brief Check whether websocket is connected at url
    *
    * @return true if connected, else false
    */
    bool isConnected() const;

    /**
    * @brief Check whether user is logged in
    *
    * @return true if user is logged in, else false
    */
    bool isLoggedIn() const;


    /**
    * @brief Reconnects the websocket to new url
    */
    void onServerURLChange();

    /**
    * @brief Returns the queue used to cache unsent messages
    *
    *@return QQueue<QPair<QString,QJsonDocument>>, The m_messageQueue object
    */
    QQueue<QPair<QString,QJsonDocument>> messageQueue();

    /**
    * @brief Returns standard cache path
    *
    *@def QString path
    */
    QString cachePath() const;

signals:
    void connectedChanged();
    void loginStatusChanged();
    void loginTypeChanged();
    void disconnected();
    void added(QJsonObject item);
    void changed(QJsonObject item);

    /**
     * @brief Emitted whenever a result is received
     *
     * @param id The ID received in the method() call
     * @param result The response sent by server
     */
    void result(unsigned id, QJsonDocument result);

private slots:
    void onWSConnected();
    void onTextMessageReceived(QString message);
    void onWSclosed();

private:

    LoginStatus loginStatus() const;
    void setLoginStatus(LoginStatus l);

    LoginType loginType() const;
    Q_INVOKABLE void setLoginType(LoginType t);

    void resume_login_callback(QJsonDocument doc);

    QString m_url;
    QWebSocket m_webSocket;

    /**
     * @brief Unique message ID for each message sent over network
     */
    unsigned m_uid;

    /**
     * @brief Stores callback function associated with each message
     *
     * @def QHash unsigned messageID and std::function<void (QJsonDocument)> pointer to callback
     */
    QHash <unsigned, std::function<void (QJsonDocument)> > m_callbackHash;

    unsigned m_loginJob;
    LoginStatus m_loginStatus;
    LoginType m_loginType;

    bool m_connected;

    bool m_attemptedPasswordLogin;
    bool m_attemptedTokenLogin;

    /**
    * @brief Abstract queue for all requests regarding network management
    *
    * @def QPair QString method and QJsonDocument params
    */
    QQueue<QPair<QString,QJsonDocument>> m_messageQueue;

    friend class Ruqola;
};


// #include "ddpclient.moc"

#endif // DDPCLIENT_H
