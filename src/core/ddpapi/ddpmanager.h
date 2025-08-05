/*

 * SPDX-FileCopyrightText: 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

class QObject;

#include "authenticationmanager/authenticationmanagerbase.h"
#include "libruqolacore_export.h"

class DDPClient;

/**
 * @class DDPManager
 * @brief Abstract class describing the interface for classes that may subscribe to DDP events
 *   and call DDP methods.
 */
class LIBRUQOLACORE_EXPORT DDPManager : public AuthenticationManagerBase
{
    Q_OBJECT

public:
    explicit DDPManager(DDPClient *ddpClient, QObject *parent = nullptr);
    ~DDPManager() override = default;

    void processMethodResponse(int opId, const QJsonObject &response);
    void processSubscriptionResult(int subId, const QJsonObject &result);

    [[nodiscard]] DDPClient *ddpClient() const;

protected:
    DDPClient *mDdpClient = nullptr;

private:
    virtual void processSubscriptionResultImpl(int subId, const QJsonObject &result) = 0;
};
