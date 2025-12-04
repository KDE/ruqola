/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] QString publicKey() const;
    void setPublicKey(const QString &newPublicKey);

private:
    QString mBinaryPrivateKey;
    QString mPublicKey;
};
QT_DECL_METATYPE_EXTERN_TAGGED(E2eKey, Ruqola_E2eKey, LIBRUQOLACORE_EXPORT)
Q_DECLARE_TYPEINFO(E2eKey, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const E2eKey &t);
