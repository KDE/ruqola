/*

 * SPDX-FileCopyrightText: 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "ddpapi/ddpmanager.h"

#include "ddpapi/ddpclient.h"

#include <QJsonObject>

DDPManager::DDPManager(DDPClient *ddpClient, QObject *parent)
    : QObject(parent)
    , mDdpClient(ddpClient)
{
}

DDPClient *DDPManager::ddpClient() const
{
    return mDdpClient;
}

void DDPManager::processMethodResponse(int opId, const QJsonObject &response)
{
    // TODO: any generic preprocessing goes here
    processMethodResponseImpl(opId, response);
}

void DDPManager::processSubscriptionResult(int subId, const QJsonObject &result)
{
    // TODO: any generic preprocessing goes here
    processSubscriptionResultImpl(subId, result);
}

#include "moc_ddpmanager.cpp"
