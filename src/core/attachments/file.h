/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
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

    [[nodiscard]] QString userId() const;
    void setUserId(const QString &userId);

    [[nodiscard]] QString url() const;
    void setUrl(const QString &url);

    [[nodiscard]] QString mimeType() const;
    void setMimeType(const QString &mimeType);

    [[nodiscard]] qint64 uploadedAt() const;
    void setUploadedAt(qint64 uploadedAt);

    [[nodiscard]] QString fileId() const;
    void setFileId(const QString &fileId);

    [[nodiscard]] QString rid() const;
    void setRid(const QString &rid);

    [[nodiscard]] QString userName() const;
    void setUserName(const QString &userName);

    [[nodiscard]] QString uploadedDateTimeStr() const;

    [[nodiscard]] bool complete() const;
    void setComplete(bool complete);

    [[nodiscard]] QString typeGroup() const;
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(File)
#endif
Q_DECLARE_TYPEINFO(File, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const File &t);
