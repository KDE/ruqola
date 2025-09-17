/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "encryption/encryptionutils.h"
#include <QNetworkAccessManager>
#include <QString>
#include <functional>

namespace UploadDownloadRsaKeyPair
{
void uploadKeys(const QString &authToken,
                const QString &url,
                const QString &userId,
                const QString &password,
                QNetworkAccessManager *networkManager,
                const std::function<void(QString, EncryptionUtils::RSAKeyPair)> &onSuccess);

void downloadKeys(const QString &authToken,
                  const QString &url,
                  const QString &userId,
                  const QString &password,
                  QNetworkAccessManager *networkManager,
                  const std::function<void(QString, QString)> &onSuccess);
}
