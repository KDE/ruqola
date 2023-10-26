/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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
    case AccountName:
        return info.accountName();
    case DateTime:
        return info.dateTime();
    case MessageStr:
        return generateMessage(info);
    case RoomId:
        return info.roomId();
    case ChannelType:
        return info.channelType();
    case Pixmap:
        return info.pixmap();
    case SenderName:
        return info.senderName();
    case SenderUserName:
        return info.senderUserName();
    case MessageId:
        return info.messageId();
    case RoomName:
        return info.roomName();
    }
    return {};
}

QString ModerationReportInfoModel::generateMessage(const NotificationInfo &info) const
{
    switch (info.notificationType()) {
    case NotificationInfo::StandardMessage:
        return info.message();
    case NotificationInfo::ConferenceCall:
        return i18n("Conference Call");
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

void ModerationReportInfoModel::insertModerationReportInfo(const QVector<NotificationInfo> &infos)
{
    clear();
    if (!infos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, infos.count() - 1);
        mModerationReportInfos = infos;
        endInsertRows();
    }
}

#include "moc_moderationreportinfomodel.cpp"
