/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamcompletermodel.h"
#include <QModelIndex>

TeamCompleterModel::TeamCompleterModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

TeamCompleterModel::~TeamCompleterModel() = default;

void TeamCompleterModel::clear()
{
    if (!mTeams.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        mTeams.clear();
        endRemoveRows();
    }
}

void TeamCompleterModel::insertTeams(const QVector<TeamCompleter> &rooms)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mTeams.count() - 1);
        mTeams.clear();
        endRemoveRows();
    }
    if (!rooms.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, rooms.count() - 1);
        mTeams = rooms;
        endInsertRows();
    }
}

int TeamCompleterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mTeams.count();
}

QVariant TeamCompleterModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mTeams.count()) {
        return {};
    }
    const TeamCompleter &team = mTeams.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case TeamName:
        return team.fname();
    case TeamId:
        return team.teamId();
    case Qt::DecorationRole:
    case TeamIcon:
        return {};
    }

    return {};
}
