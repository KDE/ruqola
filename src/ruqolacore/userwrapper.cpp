/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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


#include "userwrapper.h"
#include "user.h"
UserWrapper::UserWrapper(QObject *parent)
    : QObject(parent)
{

}

UserWrapper::UserWrapper(User *user, QObject *parent)
    : QObject(parent)
{
    mUser = user;
    qDebug() << " mUSer " << *user;
    connect(mUser, &User::nameChanged, this, &UserWrapper::nameChanged);
    connect(mUser, &User::userIdChanged, this, &UserWrapper::userIdChanged);
    connect(mUser, &User::statusChanged, this, &UserWrapper::statusChanged);
    connect(mUser, &User::userNameChanged, this, &UserWrapper::userNameChanged);
    connect(mUser, &User::utcOffsetChanged, this, &UserWrapper::utcOffsetChanged);
    connect(mUser, &User::statusTextChanged, this, &UserWrapper::statusTextChanged);
}

UserWrapper::~UserWrapper()
{

}

QString UserWrapper::name() const
{
    return mUser->name();
}

QString UserWrapper::userId() const
{
    return mUser->userId();
}

QString UserWrapper::status() const
{
    return mUser->status();
}

QString UserWrapper::userName() const
{
    return mUser->userName();
}

double UserWrapper::utcOffset() const
{
    return mUser->utcOffset();
}

QString UserWrapper::statusText() const
{
    return mUser->statusText();
}
