/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "multistaticselectlineeditmodel.h"

MultiStaticSelectLineEditModel::MultiStaticSelectLineEditModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

MultiStaticSelectLineEditModel::~MultiStaticSelectLineEditModel() = default;

int MultiStaticSelectLineEditModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mUserCompletionInfos.count();
}

QVariant MultiStaticSelectLineEditModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mUserCompletionInfos.count()) {
        return {};
    }
    const SelectItemCompletionInfo info = mUserCompletionInfos.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case Text:
        return info.text;
    case Value:
        return info.value;
    }
    return {};
}

QList<MultiStaticSelectLineEditModel::SelectItemCompletionInfo> MultiStaticSelectLineEditModel::userCompletionInfos() const
{
    return mUserCompletionInfos;
}

void MultiStaticSelectLineEditModel::setUserCompletionInfos(const QList<SelectItemCompletionInfo> &newUserCompletionInfos)
{
    beginInsertRows(QModelIndex(), 0, newUserCompletionInfos.count() - 1);
    mUserCompletionInfos = newUserCompletionInfos;
    endInsertRows();
}

bool MultiStaticSelectLineEditModel::SelectItemCompletionInfo::isValid() const
{
    return value.isEmpty() && text.isEmpty();
}

#include "moc_multistaticselectlineeditmodel.cpp"
