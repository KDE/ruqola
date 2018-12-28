/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef UNITYSERVICEMANAGER_H
#define UNITYSERVICEMANAGER_H

#include "libruqola_private_export.h"
#include <QObject>
class QDBusServiceWatcher;
class LIBRUQOLACORE_TESTS_EXPORT UnityServiceManager : public QObject
{
    Q_OBJECT
public:
    explicit UnityServiceManager(QObject *parent = nullptr);
    ~UnityServiceManager();

    void setCount(int count);

private:
    Q_DISABLE_COPY(UnityServiceManager)
    void initUnity();
    void updateCount();
    QDBusServiceWatcher *mUnityServiceWatcher = nullptr;
    int mCount = 0;
    bool mUnityServiceAvailable = false;
};
#endif // UNITYSERVICEMANAGER_H
