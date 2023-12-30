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
        beginResetModel();
        mTeams.clear();
        endResetModel();
    }
}

void TeamCompleterModel::insertTeams(const QVector<TeamCompleter> &teams)
{
    clear();
    if (!teams.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, teams.count() - 1);
        mTeams = teams;
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

#include "moc_teamcompletermodel.cpp"
