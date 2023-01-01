/*
  SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "networkmanager.h"
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include "pimcommontexttranslator_debug.h"
#include <QNetworkInformation>
#else
#include <QNetworkConfigurationManager>
#endif
using namespace PimCommonTextTranslator;

Q_GLOBAL_STATIC(NetworkManager, s_pNetworkManagerSelf)

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    , mNetworkConfigureManager(new QNetworkConfigurationManager())
#endif
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QT_WARNING_PUSH
    QT_WARNING_DISABLE_CLANG("-Wdeprecated-declarations")
    QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
    connect(mNetworkConfigureManager, &QNetworkConfigurationManager::onlineStateChanged, this, &NetworkManager::networkStatusChanged);
    QT_WARNING_POP
#else
    QNetworkInformation::instance()->load(QNetworkInformation::Feature::Reachability);
    connect(QNetworkInformation::instance(), &QNetworkInformation::reachabilityChanged, this, [this](QNetworkInformation::Reachability newReachability) {
        Q_EMIT networkStatusChanged(newReachability == QNetworkInformation::Reachability::Online);
    });
#endif
}

NetworkManager::~NetworkManager()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    delete mNetworkConfigureManager;
#endif
}

NetworkManager *NetworkManager::self()
{
    return s_pNetworkManagerSelf();
}

bool NetworkManager::isOnline() const
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 1, 0)
    if (QNetworkInformation::load(QNetworkInformation::Feature::Reachability)) {
        return QNetworkInformation::instance()->reachability() == QNetworkInformation::Reachability::Online;
    } else {
        qCWarning(PIMCOMMONTEXTTRANSLATOR_LOG) << "Couldn't find a working backend for QNetworkInformation";
        return false;
    }
#else
    return mNetworkConfigureManager->isOnline();
#endif
}
