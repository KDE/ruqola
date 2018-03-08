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
#include "typingnotification.h"
#include "ddpapi/ddpclient.h"
#include "notification.h"
#include "messagequeue.h"
#include "ruqola_debug.h"
#include "rocketchataccount.h"
#include "accountmanager.h"
#include "managerdatapaths.h"
#include "aboutdata/qmlaboutdata.h"
#include "restapi/restapirequest.h"
#include <KNotification>

Ruqola *Ruqola::self()
{
    static Ruqola *s_self = nullptr;
    if (!s_self) {
        s_self = new Ruqola;
        // Create systray to show notifications on Desktop
#if !defined(Q_OS_ANDROID) || !defined(Q_OS_IOS)
        s_self->notification();
#endif
    }
    return s_self;
}

Ruqola::Ruqola(QObject *parent)
    : QObject(parent)
{
    //Initialize paths
    (void)ManagerDataPaths::self();
    mAccountManager = new AccountManager(this);
    connect(mAccountManager, &AccountManager::notification, this, &Ruqola::sendNotification);
    connect(mAccountManager, &AccountManager::updateNotification, this, &Ruqola::updateNotification);
    connect(mAccountManager, &AccountManager::logoutAccountDone, this, &Ruqola::logout);

    mRuqolaAboutData = new QmlAboutData(this);
}

AccountManager *Ruqola::accountManager() const
{
    return mAccountManager;
}


QmlAboutData *Ruqola::applicationData() const
{
    return mRuqolaAboutData;
}

RocketChatAccount *Ruqola::rocketChatAccount() const
{
    return mAccountManager->firstAccount();
}

void Ruqola::sendNotification(const QString &title, const QString &message, const QPixmap &pixmap)
{
    KNotification::event(KNotification::Notification, title,
                         message, pixmap,
                         nullptr,
                         KNotification::Persistent | KNotification::SkipGrouping);
}

Notification *Ruqola::notification()
{
    if (!mNotification) {
        mNotification = new Notification();
    }
    return mNotification;
}

void Ruqola::updateNotification(bool hasAlert, int nbUnread, const QString &accountName)
{
    notification()->updateNotification(hasAlert, nbUnread, accountName);
}

void Ruqola::logout(const QString &accountName)
{
    notification()->clearNotification(accountName);
}
