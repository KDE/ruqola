/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "servererrorinfo.h"
#include <QObject>
class ServerErrorInfoHistoryModel;

class LIBRUQOLACORE_EXPORT ServerErrorInfoHistoryManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerErrorInfoHistoryManager(QObject *parent = nullptr);
    ~ServerErrorInfoHistoryManager() override;

    static ServerErrorInfoHistoryManager *self();

    Q_REQUIRED_RESULT ServerErrorInfoHistoryModel *notificationHistoryModel() const;

    void addNotification(const ServerErrorInfo &info);

Q_SIGNALS:
    void newNotification();

private:
    ServerErrorInfoHistoryModel *const mServerErrorInfoHistoryModel;
};
