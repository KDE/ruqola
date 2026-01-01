/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSharedData>
class QJsonObject;

class LIBRUQOLACORE_EXPORT MessagePinned : public QSharedData
{
public:
    MessagePinned();
    ~MessagePinned();
    explicit MessagePinned(const MessagePinned &other);

    [[nodiscard]] bool pinned() const;
    void setPinned(bool pinned);

    [[nodiscard]] QString pinnedBy() const;
    void setPinnedBy(const QString &pinnedBy);

    [[nodiscard]] bool operator==(const MessagePinned &other) const;
    void parse(const QJsonObject &o);

    [[nodiscard]] static QJsonObject serialize(const MessagePinned &message);
    [[nodiscard]] static MessagePinned *deserialize(const QJsonObject &o);

    [[nodiscard]] bool isValid() const;

private:
    QString mPinnedBy;
    bool mPinned = false;
};
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessagePinned &t);
