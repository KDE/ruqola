/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelusercompleter.h"
#include "libruqolacore_export.h"
#include <QAbstractListModel>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT InputCompleterModel : public QAbstractListModel
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
        Identifier,
    };
    Q_ENUM(InputCompleterRoles)

    struct SearchInfo {
        enum SearchType {
            Unknown,
            Users,
            Channels,
            ChannelsAndUsers,
        };
        SearchType searchType = SearchType::Unknown;
        QString searchString;
    };

    explicit InputCompleterModel(RocketChatAccount *account, QObject *parent = nullptr);
    ~InputCompleterModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setChannels(const QList<ChannelUserCompleter> &channels);

    void parseChannels(const QJsonObject &obj);
    void parseSearchChannels(const QJsonObject &obj);

    void clear();

    void setDefaultUserCompletion();

    void setSearchInfo(const SearchInfo &newSearchInfo);

private:
    [[nodiscard]] static ChannelUserCompleter createHereChannel();
    [[nodiscard]] static ChannelUserCompleter createAllChannel();
    [[nodiscard]] static ChannelUserCompleter noFoundChannelUser();
    [[nodiscard]] static QString here();
    [[nodiscard]] static QString all();
    [[nodiscard]] QList<ChannelUserCompleter> searchOpenedRooms();
    SearchInfo mSearchInfo;
    QList<ChannelUserCompleter> mChannelUserCompleters;
    RocketChatAccount *const mRocketChatAccount;
};
