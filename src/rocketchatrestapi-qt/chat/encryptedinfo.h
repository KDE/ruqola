/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include <QByteArray>
#include <QString>
class QJsonObject;
namespace RocketChatRestApi
{
struct LIBROCKETCHATRESTAPI_QT_EXPORT EncryptedInfo {
    QByteArray algorithm;
    QByteArray keyId;
    QString ciphertext; // TODO QByteArray ?
    QByteArray iv;
    [[nodiscard]] QJsonObject generateJson() const;
    [[nodiscard]] bool isValid() const;
};
}
