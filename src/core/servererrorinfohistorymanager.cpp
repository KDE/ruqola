/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfohistorymanager.h"
using namespace Qt::Literals::StringLiterals;

#include "model/servererrorinfohistorymodel.h"

ServerErrorInfoHistoryManager::ServerErrorInfoHistoryManager(QObject *parent)
    : QObject{parent}
    , mServerErrorInfoHistoryModel(new ServerErrorInfoHistoryModel(this))
{
}

ServerErrorInfoHistoryManager::~ServerErrorInfoHistoryManager() = default;

ServerErrorInfoHistoryManager *ServerErrorInfoHistoryManager::self()
{
    static ServerErrorInfoHistoryManager s_self;
    return &s_self;
}

ServerErrorInfoHistoryModel *ServerErrorInfoHistoryManager::serverErrorInfoHistoryModel() const
{
    return mServerErrorInfoHistoryModel;
}

void ServerErrorInfoHistoryManager::addServerErrorInfo(const ServerErrorInfo &info)
{
    mServerErrorInfoHistoryModel->addServerErrorInfo(info);
    Q_EMIT newServerErrorInfo();
}

#include "moc_servererrorinfohistorymanager.cpp"
