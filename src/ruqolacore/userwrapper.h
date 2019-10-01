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

#ifndef USERWRAPPER_H
#define USERWRAPPER_H

#include <QObject>
#include "libruqolacore_export.h"
class User;

class LIBRUQOLACORE_EXPORT UserWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString userId READ userId NOTIFY userIdChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString userName READ userName NOTIFY userNameChanged)
    Q_PROPERTY(double utcOffset READ utcOffset NOTIFY utcOffsetChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)
public:
    explicit UserWrapper(QObject *parent = nullptr);
    explicit UserWrapper(User *user, QObject *parent = nullptr);
    ~UserWrapper();

    Q_REQUIRED_RESULT QString name() const;
    Q_REQUIRED_RESULT QString userId() const;
    Q_REQUIRED_RESULT QString status() const;
    Q_REQUIRED_RESULT QString userName() const;
    Q_REQUIRED_RESULT double utcOffset() const;
    Q_REQUIRED_RESULT QString statusText() const;
Q_SIGNALS:
    void nameChanged();
    void userIdChanged();
    void statusChanged();
    void userNameChanged();
    void utcOffsetChanged();
    void statusTextChanged();
private:
    Q_DISABLE_COPY(UserWrapper)
    User *mUser = nullptr;
};

#endif // USERWRAPPER_H
