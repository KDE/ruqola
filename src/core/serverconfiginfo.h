/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#ifndef SERVERCONFIGINFO_H
#define SERVERCONFIGINFO_H

#include <QObject>
#include "libruqolacore_export.h"
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT ServerConfigInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString serverVersionStr READ serverVersionStr CONSTANT)
    Q_PROPERTY(QString serverName READ serverName CONSTANT)
    Q_PROPERTY(QString userName READ userName CONSTANT)

public:
    explicit ServerConfigInfo(RocketChatAccount *account = nullptr, QObject *parent = nullptr);
    ~ServerConfigInfo() override;
    Q_REQUIRED_RESULT QString serverVersionStr() const;
    Q_REQUIRED_RESULT QString serverName() const;
    Q_REQUIRED_RESULT QString userName() const;
private:
    Q_DISABLE_COPY(ServerConfigInfo)
    RocketChatAccount *mAccount = nullptr;
};

#endif // SERVERCONFIGINFO_H
