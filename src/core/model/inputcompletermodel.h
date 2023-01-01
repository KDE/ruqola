/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channel.h"
#include "libruqola_private_export.h"
#include <QAbstractListModel>
#include <QJsonObject>

class LIBRUQOLACORE_TESTS_EXPORT InputCompleterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum InputCompleterRoles {
        DisplayName = Qt::UserRole + 1,
        CompleterName, // keep value in sync with EmoticonModel
        IconName,
        ChannelType,
    };
    Q_ENUM(InputCompleterRoles)

    explicit InputCompleterModel(QObject *parent = nullptr);
    ~InputCompleterModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setChannels(const QVector<Channel> &channels);

    void parseChannels(const QJsonObject &obj);

    void clear();

    void setDefaultUserCompletion();

private:
    Q_DISABLE_COPY(InputCompleterModel)
    Q_REQUIRED_RESULT QIcon channelIconName(const Channel &channel) const;
    Q_REQUIRED_RESULT QString completerName(const Channel &channel) const;
    Q_REQUIRED_RESULT QString channelName(const Channel &channel) const;
    QVector<Channel> mChannel;
};
