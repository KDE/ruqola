/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "multistaticselectlineeditmodel.h"
#include "ruqola_autogenerateui_debug.h"

MultiStaticSelectLineEditModel::MultiStaticSelectLineEditModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

MultiStaticSelectLineEditModel::~MultiStaticSelectLineEditModel() = default;

int MultiStaticSelectLineEditModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) { // flat model
        return 0;
    }
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
    return !value.isEmpty() && !text.isEmpty();
}

MultiStaticSelectLineEditModel::SelectItemCompletionInfo MultiStaticSelectLineEditModel::itemCompletionInfo(const QString &value) const
{
    for (const auto &info : mUserCompletionInfos) {
        if (info.value == value) {
            return info;
        }
    }
    qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "Impossible to find info for value: " << value;
    return {};
}

#include "moc_multistaticselectlineeditmodel.cpp"
