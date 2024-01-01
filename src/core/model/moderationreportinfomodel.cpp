/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfomodel.h"
#include <KLocalizedString>

ModerationReportInfoModel::ModerationReportInfoModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

ModerationReportInfoModel::~ModerationReportInfoModel() = default;

int ModerationReportInfoModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mModerationReportInfos.count();
}

QVariant ModerationReportInfoModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mModerationReportInfos.count()) {
        return {};
    }
    const auto info = mModerationReportInfos.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case Message:
        return info.description();
    case ReportIdentifier:
        return info.reportIdentifier();
    case ReportUserName:
        return info.userName();
    case DateTime:
        return info.timeStampDateTimeStr();
    }
    return {};
}

void ModerationReportInfoModel::clear()
{
    if (rowCount() != 0) {
        beginResetModel();
        mModerationReportInfos.clear();
        endResetModel();
    }
}

void ModerationReportInfoModel::addModerationReportInfos(const ModerationReportInfos &moderationReportInfos)
{
    clear();
    if (!moderationReportInfos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, moderationReportInfos.count() - 1);
        mModerationReportInfos = moderationReportInfos;
        endInsertRows();
    }
}

#include "moc_moderationreportinfomodel.cpp"
