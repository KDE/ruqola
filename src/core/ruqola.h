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

#ifndef RUQOLA_H
#define RUQOLA_H

#include <QObject>
#include "libruqolacore_export.h"
#include <QQmlEngine>
#include <QJSEngine>
#include "ddpapi/ddpclient.h"
#include "model/roommodel.h"
#include "model/messagemodel.h"
#include "notification.h"

class QmlAboutData;

class RocketChatAccount;
class AccountManager;
class LIBRUQOLACORE_EXPORT Ruqola : public QObject
{
    Q_OBJECT
public:

    /**
    * @brief Singleton provider
    *
    * @return Returns the singleton object
    */
    static Ruqola *self();

    static void destroy();

    Notification *notification();

    Q_INVOKABLE RocketChatAccount *rocketChatAccount() const;

    Q_INVOKABLE QmlAboutData *applicationData() const;

    Q_INVOKABLE AccountManager *accountManager() const;

    explicit Ruqola(QObject *parent = nullptr);

    void setCurrentAccount(const QString &accountName);
private:
    Q_DISABLE_COPY(Ruqola)
    void sendNotification(const QString &title, const QString &message, const QPixmap &pixmap);
    void updateNotification(bool hasAlert, int nbUnread, const QString &accountName);
    void logout(const QString &accountName);
    Notification *mNotification = nullptr;
    QmlAboutData *mRuqolaAboutData = nullptr;
    AccountManager *mAccountManager = nullptr;
};

inline static QObject *ruqola_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    Ruqola *userData = Ruqola::self();
    return userData;
}

#endif // RUQOLA_H
