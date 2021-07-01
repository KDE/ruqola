/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "admincustomemojimodel.h"
#include <KLocalizedString>

AdminCustomEmojiModel::AdminCustomEmojiModel(QObject *parent)
    : DirectoryBaseModel(parent)
{
}

AdminCustomEmojiModel::~AdminCustomEmojiModel()
{
}

int AdminCustomEmojiModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mCustomSounds.count();
}

QVariant AdminCustomEmojiModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<CustomEmojiRoles>(section)) {
        case CustomEmojiRoles::Name:
            return i18n("Name");
        case CustomEmojiRoles::Identifier:
            return i18n("Identifier");
        }
    }
    return QVariant();
}

int AdminCustomEmojiModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(CustomEmojiRoles::LastColumn) + 1;
}

QVariant AdminCustomEmojiModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mCustomSounds.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const CustomSoundInfo &customSound = mCustomSounds.at(index.row());
    const int col = index.column();
    switch (static_cast<CustomEmojiRoles>(col)) {
    case CustomEmojiRoles::Name:
        return customSound.name();
    case CustomEmojiRoles::Identifier:
        return customSound.identifier();
    }

    return {};
}

int AdminCustomEmojiModel::total() const
{
    return mCustomSounds.count();
}

void AdminCustomEmojiModel::parseElements(const QJsonObject &obj)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mCustomSounds.count() - 1);
        mCustomSounds.clear();
        endRemoveRows();
    }
    mCustomSounds.parseCustomSounds(obj);
    if (!mCustomSounds.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mCustomSounds.count() - 1);
        endInsertRows();
    }
    checkFullList();
    Q_EMIT totalChanged();
}

void AdminCustomEmojiModel::checkFullList()
{
    setHasFullList(mCustomSounds.count() == mCustomSounds.total());
}

const CustomSoundsInfo &AdminCustomEmojiModel::customSounds() const
{
    return mCustomSounds;
}

void AdminCustomEmojiModel::setCustomSounds(const CustomSoundsInfo &newCustomSounds)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mCustomSounds.count() - 1);
        mCustomSounds.clear();
        endRemoveRows();
    }
    if (!mCustomSounds.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mCustomSounds.count() - 1);
        mCustomSounds = newCustomSounds;
        endInsertRows();
    }
}

void AdminCustomEmojiModel::addMoreElements(const QJsonObject &obj)
{
    const int numberOfElement = mCustomSounds.count();
    mCustomSounds.parseCustomSounds(obj);
    beginInsertRows(QModelIndex(), numberOfElement, mCustomSounds.count() - 1);
    endInsertRows();
    checkFullList();
}

QList<int> AdminCustomEmojiModel::hideColumns() const
{
    return {CustomEmojiRoles::Identifier};
}
