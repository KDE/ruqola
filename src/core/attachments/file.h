/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QString>

class LIBRUQOLACORE_EXPORT File
{
    Q_GADGET
public:
    File();
    File(const File &other) = default;

    void parseFile(const QJsonObject &json, bool restApi);
    Q_REQUIRED_RESULT QString fileName() const;
    void setFileName(const QString &fileName);

    Q_REQUIRED_RESULT QString description() const;
    void setDescription(const QString &description);

    Q_REQUIRED_RESULT bool operator==(const File &other) const;

    File &operator=(const File &other) = default;

    Q_REQUIRED_RESULT QString userId() const;
    void setUserId(const QString &userId);

    Q_REQUIRED_RESULT QString url() const;
    void setUrl(const QString &url);

    Q_REQUIRED_RESULT QString mimeType() const;
    void setMimeType(const QString &mimeType);

    Q_REQUIRED_RESULT qint64 uploadedAt() const;
    void setUploadedAt(qint64 uploadedAt);

    Q_REQUIRED_RESULT QString fileId() const;
    void setFileId(const QString &fileId);

    Q_REQUIRED_RESULT QString rid() const;
    void setRid(const QString &rid);

    Q_REQUIRED_RESULT QString userName() const;
    void setUserName(const QString &userName);

    Q_REQUIRED_RESULT QString uploadedDateTimeStr() const;

    Q_REQUIRED_RESULT bool complete() const;
    void setComplete(bool complete);

    Q_REQUIRED_RESULT QString typeGroup() const;
    void setTypeGroup(const QString &typeGroup);

private:
    QString mFileName;
    QString mDescription;
    QString mUserId;
    QString mUrl;
    QString mMimeType;
    QString mFileId;
    QString mRid;
    QString mUserName;
    QString mUploadedDateTimeStr;
    QString mTypeGroup;

    bool mComplete = false;
    qint64 mUploadedAt = -1;
};
Q_DECLARE_METATYPE(File)
Q_DECLARE_TYPEINFO(File, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const File &t);
