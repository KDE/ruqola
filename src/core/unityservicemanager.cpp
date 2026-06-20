/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "unityservicemanager.h"

#include "ruqola_unity_manager_debug.h"
#include <QApplication>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusMessage>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QDBusServiceWatcher>

using namespace Qt::Literals::StringLiterals;
UnityServiceManager::UnityServiceManager(QObject *parent)
    : QObject(parent)
    , mUnityServiceWatcher(new QDBusServiceWatcher(this))
{
    qCDebug(RUQOLA_UNITY_MANAGER_LOG) << "UnityServiceManager::UnityServiceManager" << this;
    initUnity();
}

UnityServiceManager::~UnityServiceManager() = default;

void UnityServiceManager::updateCount()
{
    qCDebug(RUQOLA_UNITY_MANAGER_LOG) << "UnityServiceManager::updateCount. mUnityServiceAvailable:" << mUnityServiceAvailable;
    if (mUnityServiceAvailable) {
        const QString launcherId = qApp->desktopFileName() + ".desktop"_L1;

        const QVariantMap properties{{u"count-visible"_s, mCount > 0}, {u"count"_s, mCount}};

        QDBusMessage message = QDBusMessage::createSignal(u"/org/ruqola/UnityLauncher"_s, u"com.canonical.Unity.LauncherEntry"_s, u"Update"_s);
        message.setArguments({launcherId, properties});
        if (!QDBusConnection::sessionBus().send(message)) {
            qCWarning(RUQOLA_UNITY_MANAGER_LOG) << "Impossible to send message on dbus";
        }
    }
}

void UnityServiceManager::setCount(int count)
{
    if (mCount != count) {
        mCount = count;
        updateCount();
    }
}

void UnityServiceManager::initUnity()
{
    mUnityServiceWatcher->setConnection(QDBusConnection::sessionBus());
    mUnityServiceWatcher->setWatchMode(QDBusServiceWatcher::WatchForUnregistration | QDBusServiceWatcher::WatchForRegistration);
    mUnityServiceWatcher->addWatchedService(u"com.canonical.Unity"_s);
    connect(mUnityServiceWatcher, &QDBusServiceWatcher::serviceRegistered, this, [this]([[maybe_unused]] const QString &service) {
        qCWarning(RUQOLA_UNITY_MANAGER_LOG) << "Unity Service Registered";
        mUnityServiceAvailable = true;
        updateCount();
    });

    connect(mUnityServiceWatcher, &QDBusServiceWatcher::serviceUnregistered, this, [this]([[maybe_unused]] const QString &service) {
        qCWarning(RUQOLA_UNITY_MANAGER_LOG) << "Unity Service Unregistered";
        mUnityServiceAvailable = false;
    });

    // QDBusConnectionInterface::isServiceRegistered blocks
    const QDBusPendingCall listNamesCall = QDBusConnection::sessionBus().interface()->asyncCall(u"ListNames"_s);
    auto callWatcher = new QDBusPendingCallWatcher(listNamesCall, this);
    connect(callWatcher, &QDBusPendingCallWatcher::finished, this, [this](QDBusPendingCallWatcher *watcher) {
        const QDBusPendingReply<QStringList> reply = *watcher;
        watcher->deleteLater();

        if (reply.isError()) {
            qCWarning(RUQOLA_UNITY_MANAGER_LOG) << " reply" << reply.error().message();
        }

        const QStringList &services = reply.value();

        mUnityServiceAvailable = services.contains("com.canonical.Unity"_L1);
        qCWarning(RUQOLA_UNITY_MANAGER_LOG) << " mUnityServiceAvailable changed" << mUnityServiceAvailable;
        if (mUnityServiceAvailable) {
            updateCount();
        }
    });
}

#include "moc_unityservicemanager.cpp"
