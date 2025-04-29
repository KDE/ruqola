/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "aitextmodel.h"

AiTextModel::AiTextModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

AiTextModel::~AiTextModel() = default;

int AiTextModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mTextInfos.count();
}

QVariant AiTextModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mTextInfos.count()) {
        return {};
    }

    const auto &textInfo = mTextInfos[index.row()];
    switch (role) {
    case Qt::DisplayRole:
    case TextRole:
        return textInfo.requestText();
    case Qt::CheckStateRole:
    case EnabledRole:
        return textInfo.enabled() ? Qt::Checked : Qt::Unchecked;
    }
    return {};
}

bool AiTextModel::setData(const QModelIndex &idx, const QVariant &value, int role)
{
    if (!idx.isValid()) {
        // qCWarning(TEXTAUTOGENERATETEXT_CORE_LOG) << "ERROR: invalid index";
        return false;
    }
    const int id = idx.row();
    AiTextInfo &info = mTextInfos[id];
    switch (role) {
    case AiTextRoles::TextRole: {
        info.setRequestText(value.toString());
        const QModelIndex newIndex = index(idx.row(), AiTextRoles::TextRole);
        Q_EMIT dataChanged(newIndex, newIndex);
        return true;
    }
    case Qt::CheckStateRole:
    case AiTextRoles::EnabledRole:
        info.setEnabled(value.toBool());
        Q_EMIT dataChanged(idx, idx, {AiTextRoles::EnabledRole});
        return true;
    }
    return QAbstractListModel::setData(idx, value, role);
}

QList<AiTextInfo> AiTextModel::infos() const
{
    return mTextInfos;
}

void AiTextModel::setInfos(const QList<AiTextInfo> &newAskItems)
{
    beginResetModel();
    mTextInfos = newAskItems;
    endResetModel();
}

void AiTextModel::addItem(const AiTextInfo &msg)
{
    beginInsertRows(QModelIndex(), mTextInfos.count(), mTextInfos.count());
    mTextInfos.append(msg);
    endInsertRows();
}

Qt::ItemFlags AiTextModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | Qt::ItemIsUserCheckable | QAbstractListModel::flags(index);
}

#include "moc_aitextmodel.cpp"
