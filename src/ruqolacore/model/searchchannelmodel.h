/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef SEARCHCHANNELMODEL_H
#define SEARCHCHANNELMODEL_H

#include <QAbstractListModel>
#include <channel.h>
#include "libruqola_private_export.h"

class LIBRUQOLACORE_TESTS_EXPORT SearchChannelModel : public QAbstractListModel
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

    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;
    void setChannels(const QVector<Channel> &channels);

    void parseChannels(const QJsonObject &obj);

    void clear();

private:
    QIcon channelIconName(const Channel &channel) const;
    QString channelId(const Channel &channel) const;
    QString channelName(const Channel &channel) const;
    QVector<Channel> mChannel;
};

#endif // SEARCHCHANNELMODEL_H
