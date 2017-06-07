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

// #include <QObject>
// #include <QUrl>
// #include <QtWebSockets/QtWebSockets>

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

    DDPClient(const QString &url = QString(), QObject *parent = 0);
    ~DDPClient();

    /**
     * @brief Call a method with name @param method and parameters @param params
     *
     * @param method The name of the method
     * @param params The parameters
     * @return unsigned int, the ID of the called method. Watch for it
     */
    unsigned method(const QString &method, const QJsonDocument &params);
    unsigned method(const QString &method, const QJsonDocument &params, std::function<void (QJsonDocument)> callback);
//     unsigned method(const QString &method, const QJsonObject &params);

    void subscribe(const QString &collection, const QJsonArray &params);

    Q_INVOKABLE void login();
    void logOut();

//     Q_INVOKABLE void loginWithPassword();
    bool isConnected() const;
    bool isLoggedIn() const;

    void onServerURLChange();

    //Again try to send unsent message; returns true if message was sent successfully
    bool unsentMessages();

signals:
//     void connected();
    void connectedChanged();

    void loginStatusChanged();
//     void loggedInChanged();
    void disconnected();
    /**
     * @brief Emitted whenever a result is received. The parameter is the expected ID.
     *
     * @param id the ID received in the method() call
     */
    void result(unsigned id, QJsonDocument result);

    void added(QJsonObject item);
    void changed(QJsonObject item);

private slots:
    void onWSConnected();
    void onTextMessageReceived(QString message);
    void WSclosed();

private:

    LoginStatus loginStatus() const;
    void setLoginStatus(LoginStatus l);

    void resume_login_callback(QJsonDocument doc);

    QString m_url;
    QWebSocket m_webSocket;

    unsigned m_uid;

    QHash <unsigned, std::function<void (QJsonDocument)> > m_callbackHash;

    unsigned m_loginJob;
    LoginStatus m_loginStatus;

    bool m_connected;

    bool m_attemptedPasswordLogin;
    bool m_attemptedTokenLogin;

    //pair- int (m_uid), QJsonDocument (params)
    QQueue<QPair<int,QJsonDocument>> m_messageQueue;

    //message with m_uid sent succussfully or not
    QHash<int,bool> m_messageStatus;

    friend class Ruqola;
};


// #include "ddpclient.moc"

#endif // DDPCLIENT_H
