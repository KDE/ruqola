/*
   Copyright (C) 2018 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef FILE_H
#define FILE_H

#include "libruqola_private_export.h"
#include <QString>
#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_TESTS_EXPORT File
{
    Q_GADGET
public:
    File();

    void parseFile(const QJsonObject &json, bool restApi = false);
    Q_REQUIRED_RESULT QString fileName() const;
    void setFileName(const QString &fileName);

    Q_REQUIRED_RESULT QString description() const;
    void setDescription(const QString &description);

    Q_REQUIRED_RESULT bool operator ==(const File &other) const;

    File &operator=(const File &other);
    Q_REQUIRED_RESULT QString userId() const;
    void setUserId(const QString &userId);

    Q_REQUIRED_RESULT QString url() const;
    void setUrl(const QString &url);

    Q_REQUIRED_RESULT QString mimeType() const;
    void setMimeType(const QString &mimeType);

    Q_REQUIRED_RESULT qint64 uploadedAt() const;
    void setUploadedAt(const qint64 &uploadedAt);

    Q_REQUIRED_RESULT QString fileId() const;
    void setFileId(const QString &fileId);

    Q_REQUIRED_RESULT QString rid() const;
    void setRid(const QString &rid);

private:
    QString mName;
    QString mDescription;
    QString mUserId;
    QString mUrl;
    QString mMimeType;
    QString mFileId;
    QString mRid;
    qint64 mUploadedAt = -1;
};
Q_DECLARE_METATYPE(File)
Q_DECLARE_TYPEINFO(File, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const File &t);

#endif // FILES_H
