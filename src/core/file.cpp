/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "file.h"
#include "utils.h"
#include <QDateTime>

File::File()
{
}

void File::parseFile(const QJsonObject &object, bool restApi)
{
    const QJsonObject fields = restApi ? object : object.value(QLatin1String("fields")).toObject();
    setUserId(fields.value(QLatin1String("userId")).toString());

    setDescription(fields.value(QLatin1String("description")).toString());
    setFileName(fields.value(QLatin1String("name")).toString());
    setMimeType(fields.value(QLatin1String("type")).toString());
    setUrl(fields.value(QLatin1String("url")).toString());
    setRid(fields.value(QLatin1String("rid")).toString());
    setComplete(fields.value(QLatin1String("complete")).toBool());
    if (restApi) {
        setUploadedAt(Utils::parseIsoDate(QStringLiteral("uploadedAt"), fields));
    } else {
        setUploadedAt(Utils::parseDate(QStringLiteral("uploadedAt"), fields));
    }

    const QJsonObject user = object.value(QLatin1String("user")).toObject();
    setUserName(user.value(QLatin1String("username")).toString());

    setFileId(restApi ? object.value(QLatin1String("_id")).toString() : object.value(QLatin1String("id")).toString());
}

QString File::fileName() const
{
    return mFileName;
}

void File::setFileName(const QString &name)
{
    mFileName = name;
}

QString File::description() const
{
    return mDescription;
}

void File::setDescription(const QString &description)
{
    mDescription = description;
}

bool File::operator ==(const File &other) const
{
    return (description() == other.description())
           && (fileName() == other.fileName())
           && (url() == other.url())
           && (userId() == other.userId())
           && (mimeType() == other.mimeType())
           && (uploadedAt() == other.uploadedAt())
           && (fileId() == other.fileId())
           && (rid() == other.rid())
           && (userName() == other.userName())
           && (complete() == other.complete());
}

QString File::userId() const
{
    return mUserId;
}

void File::setUserId(const QString &userId)
{
    mUserId = userId;
}

QString File::url() const
{
    return mUrl;
}

void File::setUrl(const QString &url)
{
    mUrl = url;
}

QString File::mimeType() const
{
    return mMimeType;
}

void File::setMimeType(const QString &mimeType)
{
    mMimeType = mimeType;
}

qint64 File::uploadedAt() const
{
    return mUploadedAt;
}

void File::setUploadedAt(const qint64 &uploadedAt)
{
    mUploadedAt = uploadedAt;
    QLocale l;
    mUploadedDateTimeStr = l.toString(QDateTime::fromMSecsSinceEpoch(mUploadedAt), QLocale::LongFormat);
}

QString File::fileId() const
{
    return mFileId;
}

void File::setFileId(const QString &fileId)
{
    mFileId = fileId;
}

QString File::rid() const
{
    return mRid;
}

void File::setRid(const QString &rid)
{
    mRid = rid;
}

QString File::userName() const
{
    return mUserName;
}

void File::setUserName(const QString &userName)
{
    mUserName = userName;
}

QString File::uploadedDateTimeStr() const
{
    return mUploadedDateTimeStr;
}

bool File::complete() const
{
    return mComplete;
}

void File::setComplete(bool complete)
{
    mComplete = complete;
}

QDebug operator <<(QDebug d, const File &t)
{
    d << "Name : " << t.fileName();
    d << "Description: " << t.description();
    d << "Url :" << t.url();
    d << "UserId: " << t.userId();
    d << "Mimetype : "<< t.mimeType();
    d << "Uploaded time: " << t.uploadedAt();
    d << "File Id: " << t.fileId();
    d << "Rid: " << t.rid();
    d << "Username: " << t.userName();
    d << "Complete: " << t.complete();
    return d;
}
