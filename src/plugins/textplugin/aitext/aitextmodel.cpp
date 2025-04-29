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

    /*
    const auto &askItem = mTextInfos[index.row()];
    switch (role) {
    case Qt::DisplayRole:
    case TitleRole:
        return askItem.title();
    case TextRole:
        return askItem.text();
    case EnabledRole:
        return askItem.enabled();
    }
    */
    return {};
}

bool AiTextModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // TODO
    return false;
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
