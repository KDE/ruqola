/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QAbstractListModel>
#include <channel.h>

class LIBRUQOLACORE_EXPORT SearchChannelModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ChannelRoles {
        ChannelName = Qt::UserRole + 1,
        ChannelId,
        IconName,
        ChannelType,
    };
    Q_ENUM(ChannelRoles)

    explicit SearchChannelModel(QObject *parent = nullptr);
    ~SearchChannelModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setChannels(const QVector<Channel> &channels);

    void parseChannels(const QJsonObject &obj);

    void clear();

    void parseAllChannels(const QJsonObject &obj);

private:
    Q_REQUIRED_RESULT LIBRUQOLACORE_NO_EXPORT QIcon channelIconName(const Channel &channel) const;
    Q_REQUIRED_RESULT LIBRUQOLACORE_NO_EXPORT QString channelId(const Channel &channel) const;
    Q_REQUIRED_RESULT LIBRUQOLACORE_NO_EXPORT QString channelName(const Channel &channel) const;
    QVector<Channel> mChannel;
};
