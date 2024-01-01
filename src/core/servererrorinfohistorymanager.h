/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

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

    [[nodiscard]] ServerErrorInfoHistoryModel *serverErrorInfoHistoryModel() const;

    void addServerErrorInfo(const ServerErrorInfo &info);

Q_SIGNALS:
    void newServerErrorInfo();

private:
    ServerErrorInfoHistoryModel *const mServerErrorInfoHistoryModel;
};
