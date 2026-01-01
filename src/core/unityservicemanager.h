/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QObject>
class QDBusServiceWatcher;
class LIBRUQOLACORE_TESTS_EXPORT UnityServiceManager : public QObject
{
    Q_OBJECT
public:
    explicit UnityServiceManager(QObject *parent = nullptr);
    ~UnityServiceManager() override;

    void setCount(int count);

private:
    LIBRUQOLACORE_NO_EXPORT void initUnity();
    LIBRUQOLACORE_NO_EXPORT void updateCount();
    QDBusServiceWatcher *const mUnityServiceWatcher;
    int mCount = 0;
    bool mUnityServiceAvailable = false;
};
