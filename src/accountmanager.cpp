/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "accountmanager.h"
#include "rocketchataccount.h"
#include "managerdatapaths.h"
#include <QDir>
#include <QDirIterator>
#include <QFileInfoList>

#include "model/rocketchataccountmodel.h"

AccountManager::AccountManager(QObject *parent)
    : QObject(parent)
{
    mRocketChatAccountModel = new RocketChatAccountModel(this);
    loadAccount();
}

AccountManager::~AccountManager()
{

}

void AccountManager::loadAccount()
{
    qDebug() << " void AccountManager::loadAccount()"<<ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString());
    QDirIterator it(ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString()), QStringList() << QStringLiteral("ruqola.conf"), QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        qDebug() << " list.at(i)" << it.next();
    }
    //Load account
    //TODO add account
}

void AccountManager::addAccount(RocketChatAccount *account)
{
    mRocketChatAccountModel->insertAccount(account);
    //TODO add account
}

void AccountManager::removeAccount()
{
    //TODO add account
}

RocketChatAccountModel *AccountManager::rocketChatAccountModel() const
{
    return mRocketChatAccountModel;
}

