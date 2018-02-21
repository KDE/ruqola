/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#ifndef PLUGINAUTHENTICATION_H
#define PLUGINAUTHENTICATION_H

#include <QObject>
#include "libruqolacore_export.h"
#include "authenticationmanager.h"

class PluginAuthenticationInterface;

class LIBRUQOLACORE_EXPORT PluginAuthentication : public QObject
{
    Q_OBJECT
public:
    explicit PluginAuthentication(QObject *parent = nullptr);
    ~PluginAuthentication();

    virtual PluginAuthenticationInterface *createInterface(QObject *parent) = 0;

    virtual AuthenticationManager::OauthType type() const = 0;

    virtual QString name() const = 0;
    virtual QString iconName() const = 0;
};

#endif // PLUGINAUTHENTICATION_H
