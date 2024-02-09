/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
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
