/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef ROCKETCHATACCOUNTSETTINGS_H
#define ROCKETCHATACCOUNTSETTINGS_H

#include <QString>
#include "libruqola_private_export.h"

class LIBRUQOLACORE_TESTS_EXPORT RocketChatAccountSettings
{
public:
    RocketChatAccountSettings();

    QString userId() const;
    void setUserId(const QString &userId);

    QString authToken() const;
    void setAuthToken(const QString &authToken);

    QString serverUrl() const;
    void setServerUrl(const QString &serverUrl);

    QString accountName() const;
    void setAccountName(const QString &accountName);

    void logout();
private:
    void saveSettings();
    void loadSettings();
    QString mUserId;
    QString mAuthToken;
    QString mServerUrl;
    QString mAccountName;
};

#endif // ROCKETCHATACCOUNTSETTINGS_H
