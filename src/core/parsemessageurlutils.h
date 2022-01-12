/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QDebug>
#include <QString>

class LIBRUQOLACORE_TESTS_EXPORT ParseMessageUrlUtils
{
public:
    ParseMessageUrlUtils();
    ~ParseMessageUrlUtils();

    void parseUrl(const QString &url);

    const QString &messageId() const;
    void setMessageId(const QString &newMessageId);

    const QString &roomId() const;
    void setRoomId(const QString &newRoomId);

    const QString &serverPath() const;
    void setServerPath(const QString &newServerPath);

private:
    QString mMessageId;
    QString mRoomId;
    QString mServerPath;
};
