/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelusercompleter.h"
#include "libruqola_private_export.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_TESTS_EXPORT InputCompleterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum InputCompleterRoles {
        DisplayName = Qt::UserRole + 1,
        CompleterName, // keep value in sync with EmoticonModel
        IconStatus,
        ChannelType,
        Description,
        UserName,
        AvatarInfo,
        OutsideRoom,
    };
    Q_ENUM(InputCompleterRoles)

    explicit InputCompleterModel(QObject *parent = nullptr);
    ~InputCompleterModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setChannels(const QVector<ChannelUserCompleter> &channels);

    void parseChannels(const QJsonObject &obj);

    void clear();

    void setDefaultUserCompletion();

    void setSearchUserString(const QString &str);

private:
    Q_DISABLE_COPY(InputCompleterModel)
    [[nodiscard]] static ChannelUserCompleter createHereChannel();
    [[nodiscard]] static ChannelUserCompleter createAllChannel();
    [[nodiscard]] static ChannelUserCompleter noFoundChannelUser();
    void searchOpenedRooms();
    QString mSetSearchUserString;
    QVector<ChannelUserCompleter> mChannelUserCompleters;
};
