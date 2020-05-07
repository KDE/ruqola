/*

 * Copyright 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <QObject>

#include "libruqolacore_export.h"

class DDPClient;

/**
 * @class DDPManager
 * @brief Abstract class describing the interface for classes that may subscribe to DDP events
 *   and call DDP methods.
 */
class LIBRUQOLACORE_EXPORT DDPManager : public QObject
{
    Q_OBJECT

public:
    explicit DDPManager(DDPClient *ddpClient, QObject *parent = nullptr);
    ~DDPManager() override = default;

    void processMethodResponse(int opId, const QJsonObject &response);
    void processSubscriptionResult(int subId, const QJsonObject &result);

    Q_REQUIRED_RESULT DDPClient *ddpClient() const;

protected:
    DDPClient *mDdpClient = nullptr;

private:
    virtual void processMethodResponseImpl(int opId, const QJsonObject &response) = 0;
    virtual void processSubscriptionResultImpl(int subId, const QJsonObject &result) = 0;
};
