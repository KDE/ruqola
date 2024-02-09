/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QString>

class LIBRUQOLACORE_EXPORT E2eKey
{
public:
    E2eKey();
    ~E2eKey();

    void parseKeyInfo(const QJsonObject &replyObject);

    [[nodiscard]] QString binaryPrivateKey() const;
    void setBinaryPrivateKey(const QString &newBinaryPrivateKey);

private:
    QString mBinaryPrivateKey;
    QString mPublicKey;
};
Q_DECLARE_METATYPE(E2eKey)
Q_DECLARE_TYPEINFO(E2eKey, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const E2eKey &t);
