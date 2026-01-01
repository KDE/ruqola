/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

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
    ~ServerErrorInfoHistoryManager() override;

    static ServerErrorInfoHistoryManager *self();

    [[nodiscard]] ServerErrorInfoHistoryModel *serverErrorInfoHistoryModel() const;

    void addServerErrorInfo(const ServerErrorInfo &info);

Q_SIGNALS:
    void newServerErrorInfo();

private:
    explicit ServerErrorInfoHistoryManager(QObject *parent = nullptr);
    ServerErrorInfoHistoryModel *const mServerErrorInfoHistoryModel;
};
