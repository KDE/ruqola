/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminusersbasemodel.h"

#include <KLocalizedString>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
AdminUsersBaseModel::AdminUsersBaseModel(QObject *parent)
    : DirectoryUsersModel(parent)
{
}

AdminUsersBaseModel::~AdminUsersBaseModel() = default;

Users::ParseType AdminUsersBaseModel::parseType() const
{
    return Users::ParseType::Administrator;
}

void AdminUsersBaseModel::insertElement(const QJsonObject &obj)
{
    User user;
    const QJsonObject userObj = obj["user"_L1].toObject();
    user.parseUserRestApi(userObj, {});
    if (user.isValid()) {
        const int numberOfElement = mUsers.count();
        mUsers.appendUser(user); // TODO useful ?
        beginInsertRows(QModelIndex(), numberOfElement, mUsers.count() - 1);
        endInsertRows();
        checkFullList(); // TODO verify it
    }
}

void AdminUsersBaseModel::removeElement(const QByteArray &identifier)
{
    const int userCount = mUsers.count();
    for (int i = 0; i < userCount; ++i) {
        if (mUsers.at(i).userId() == identifier) {
            beginRemoveRows(QModelIndex(), i, i);
            mUsers.takeAt(i);
            mUsers.setTotal(mUsers.count()); // Update total
            endRemoveRows();
            Q_EMIT totalChanged();
            break;
        }
    }
}

void AdminUsersBaseModel::updateElement(const QJsonObject &userObj)
{
    const int roomCount = mUsers.count();
    const QJsonObject obj = userObj["user"_L1].toObject();
    const QByteArray identifier{obj.value("_id"_L1).toString().toLatin1()};
    for (int i = 0; i < roomCount; ++i) {
        if (mUsers.at(i).userId() == identifier) {
            beginRemoveRows(QModelIndex(), i, i);
            mUsers.takeAt(i);
            endRemoveRows();
            User newUser;
            newUser.parseUserRestApi(obj, {} /*, mRocketChatAccount->roleInfo()*/); // TODO necessary ?
            beginInsertRows(QModelIndex(), i, i);
            mUsers.insertUser(i, newUser);
            endInsertRows();
            break;
        }
    }
}

#include "moc_adminusersbasemodel.cpp"
