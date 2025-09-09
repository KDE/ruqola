/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "e2e/fetchmykeysjob.h"
#include "e2e/setuserpublicandprivatekeysjob.h"
#include "encryption/encryptionutils.h"
#include "restapimethod.h"
#include <QNetworkAccessManager>
#include <QString>
#include <functional>

void uploadKeys(const QString &authToken,
                const QString &url,
                const QString &userId,
                const QString &password,
                QNetworkAccessManager *networkManager,
                std::function<void(QString, EncryptionUtils::RSAKeyPair)> onSuccess);

void downloadKeys(const QString &authToken,
                  const QString &url,
                  const QString &userId,
                  const QString &password,
                  QNetworkAccessManager *networkManager,
                  std::function<void(QString, QString)> onSuccess);
