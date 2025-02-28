/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "previewcommandmodel.h"

PreviewCommandModel::PreviewCommandModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

PreviewCommandModel::~PreviewCommandModel() = default;

int PreviewCommandModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mPreviewCommands.count();
}

QList<PreviewCommand> PreviewCommandModel::previewCommands() const
{
    return mPreviewCommands;
}

void PreviewCommandModel::setPreviewCommands(const QList<PreviewCommand> &newPreviewCommands)
{
    clear();
    if (!newPreviewCommands.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, newPreviewCommands.count() - 1);
        mPreviewCommands = newPreviewCommands;
        endInsertRows();
    }
}

void PreviewCommandModel::clear()
{
    if (!mPreviewCommands.isEmpty()) {
        beginResetModel();
        mPreviewCommands.clear();
        endResetModel();
    }
}

QVariant PreviewCommandModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mPreviewCommands.count()) {
        return {};
    }

    const PreviewCommand &permissionInfo = mPreviewCommands.at(index.row());
    if (role == Qt::DisplayRole) {
        return permissionInfo.value();
    } else if (role == static_cast<int>(PreviewCommandRoles::PreviewCommandInfo)) {
        return QVariant::fromValue(permissionInfo);
    } else if (role == static_cast<int>(PreviewCommandRoles::Image)) {
        // TODO
        return {};
    }
    return {};
}

#include "moc_previewcommandmodel.cpp"
