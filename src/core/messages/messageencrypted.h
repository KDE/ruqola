/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSharedData>
class QJsonObject;

class QDebug;
class LIBRUQOLACORE_EXPORT MessageEncrypted : public QSharedData
{
public:
    MessageEncrypted();
    ~MessageEncrypted();
    explicit MessageEncrypted(const MessageEncrypted &other);

    [[nodiscard]] bool pinned() const;
    void setPinned(bool pinned);

    [[nodiscard]] QString pinnedBy() const;
    void setPinnedBy(const QString &pinnedBy);

    [[nodiscard]] bool operator==(const MessageEncrypted &other) const;
    void parse(const QJsonObject &o);

    [[nodiscard]] static QJsonObject serialize(const MessageEncrypted &message);
    [[nodiscard]] static MessageEncrypted *deserialize(const QJsonObject &o);

    [[nodiscard]] bool isValid() const;

private:
    QString mPinnedBy;
    bool mPinned = false;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageEncrypted &t);
