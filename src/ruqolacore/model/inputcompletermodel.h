/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#ifndef INPUTCOMPLETERMODEL_H
#define INPUTCOMPLETERMODEL_H

#include "libruqola_private_export.h"
#include "channel.h"
#include <QJsonObject>
#include <QAbstractListModel>

class LIBRUQOLACORE_TESTS_EXPORT InputCompleterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum InputCompleterRoles {
        DisplayName = Qt::UserRole + 1,
        CompleterName,
        IconName,
        ChannelType,
    };
    Q_ENUM(InputCompleterRoles)

    explicit InputCompleterModel(QObject *parent = nullptr);
    ~InputCompleterModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;
    void setChannels(const QVector<Channel> &channels);

    void parseChannels(const QJsonObject &obj);

    void clear();

private:
    Q_DISABLE_COPY(InputCompleterModel)
    QIcon channelIconName(const Channel &channel) const;
    QString completerName(const Channel &channel) const;
    QString channelName(const Channel &channel) const;
    QVector<Channel> mChannel;
};

#endif // INPUTCOMPLETERMODEL_H
