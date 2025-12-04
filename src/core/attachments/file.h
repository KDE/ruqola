/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QJsonObject>
#include <QString>

class LIBRUQOLACORE_EXPORT File
{
    Q_GADGET
public:
    File();
    File(const File &other) = default;

    void parseFile(const QJsonObject &json, bool restApi);
    [[nodiscard]] QString fileName() const;
    void setFileName(const QString &fileName);

    [[nodiscard]] QString description() const;
    void setDescription(const QString &description);

    [[nodiscard]] bool operator==(const File &other) const;

    File &operator=(const File &other) = default;

    [[nodiscard]] QByteArray userId() const;
    void setUserId(const QByteArray &userId);

    [[nodiscard]] QString url() const;
    void setUrl(const QString &url);

    [[nodiscard]] QString mimeType() const;
    void setMimeType(const QString &mimeType);

    [[nodiscard]] qint64 uploadedAt() const;
    void setUploadedAt(qint64 uploadedAt);

    [[nodiscard]] QByteArray fileId() const;
    void setFileId(const QByteArray &fileId);

    [[nodiscard]] QString userName() const;
    void setUserName(const QString &userName);

    [[nodiscard]] QString uploadedDateTimeStr() const;

    [[nodiscard]] bool complete() const;
    void setComplete(bool complete);

    [[nodiscard]] QString typeGroup() const;
    void setTypeGroup(const QString &typeGroup);

    [[nodiscard]] QString path() const;
    void setPath(const QString &newPath);

    [[nodiscard]] QByteArray roomId() const;
    void setRoomId(const QByteArray &newRoomId);

private:
    QString mFileName;
    QString mDescription;
    QByteArray mUserId;
    QString mUrl;
    QString mMimeType;
    QByteArray mFileId;
    QString mUserName;
    QString mUploadedDateTimeStr;
    QString mTypeGroup;
    QString mPath;
    QByteArray mRoomId;

    bool mComplete = false;
    qint64 mUploadedAt = -1;
};
Q_DECLARE_TYPEINFO(File, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const File &t);
