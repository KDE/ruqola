/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QDebug>
class LIBRUQOLACORE_TESTS_EXPORT MessagePinned
{
public:
    MessagePinned();

    [[nodiscard]] bool pinned() const;
    void setPinned(bool pinned);

    [[nodiscard]] QString pinnedBy() const;
    void setPinnedBy(const QString &pinnedBy);

    [[nodiscard]] bool operator==(const MessagePinned &other) const;
    void parse(const QJsonObject &o);

    static QJsonObject serialize(const MessagePinned &message);
    static MessagePinned deserialize(const QJsonObject &o);

private:
    QString mPinnedBy;
    bool mPinned = false;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessagePinned &t);
