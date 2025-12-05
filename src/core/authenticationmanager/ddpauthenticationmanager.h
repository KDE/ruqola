/*

 * SPDX-FileCopyrightText: 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include "ddpapi/ddpmanager.h"
#include "libruqolacore_export.h"

class QJsonObject;
class LIBRUQOLACORE_EXPORT DDPAuthenticationManager : public DDPManager
{
    Q_OBJECT

public:
    explicit DDPAuthenticationManager(DDPClient *ddpClient, QObject *parent = nullptr);
    ~DDPAuthenticationManager() override;

    [[nodiscard]] QByteArray authenticationName() const override;

protected:
    void callLoginImpl(const QJsonArray &params, AuthenticationManagerBase::Method method) override;

private:
    // Authentication doesn't involve any subscriptions
    LIBRUQOLACORE_NO_EXPORT void processSubscriptionResultImpl(int subscriptionId, const QJsonObject &result) override
    {
        Q_UNUSED(subscriptionId)
        Q_UNUSED(result)
    }

    LIBRUQOLACORE_NO_EXPORT void clientConnectedChangedSlot(bool connected);
};
