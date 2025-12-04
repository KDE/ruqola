/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QList>
#include <QObject>
#include <QSharedData>
class LIBRUQOLACORE_EXPORT Channels : public QSharedData
{
public:
    explicit Channels(const Channels &other);
    Channels();
    ~Channels();

    struct LIBRUQOLACORE_EXPORT ChannelInfo {
        QString fname;
        QString name;
        QByteArray identifier;
        [[nodiscard]] bool operator==(const ChannelInfo &other) const;
    };

    void setChannels(const QList<ChannelInfo> &channels);
    [[nodiscard]] QList<ChannelInfo> channels() const;

    void parseChannels(const QJsonArray &array);

    [[nodiscard]] bool operator==(const Channels &other) const;

    [[nodiscard]] static QJsonArray serialize(const Channels &channels);
    [[nodiscard]] static Channels *deserialize(const QJsonArray &o);

    [[nodiscard]] bool isEmpty() const;

private:
    QList<ChannelInfo> mChannels;
};
Q_DECLARE_METATYPE(Channels)
Q_DECLARE_TYPEINFO(Channels::ChannelInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Channels &t);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Channels::ChannelInfo &t);
