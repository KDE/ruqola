/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QObject>
class Connection;
class LIBRUQOLACORE_EXPORT AddUserInChannelJob : public QObject
{
    Q_OBJECT
public:
    struct AddUserInChannelJobInfo {
        QByteArray roomId;
        QByteArray userId;
        [[nodiscard]] bool isValid() const;
    };

    explicit AddUserInChannelJob(Connection *restApiConnection, QObject *parent = nullptr);
    ~AddUserInChannelJob() override;

    [[nodiscard]] bool canStart() const;

    void start();
    [[nodiscard]] AddUserInChannelJobInfo info() const;
    void setInfo(const AddUserInChannelJobInfo &newInfo);

private:
    AddUserInChannelJobInfo mInfo;
    Connection *const mConnection;
};
