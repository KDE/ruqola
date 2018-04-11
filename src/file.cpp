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

#include "file.h"
#include "utils.h"

File::File()
{
}

void File::parseFile(const QJsonObject &object, bool restApi)
{
    const QJsonObject fields = object.value(QLatin1String("fields")).toObject();
    setUserId(fields.value(QLatin1String("userId")).toString());

    //TODO use rid ???
    setName(fields.value(QLatin1String("name")).toString());
    setMimeType(fields.value(QLatin1String("type")).toString());
    setUrl(fields.value(QLatin1String("url")).toString());
    setRid(fields.value(QLatin1String("rid")).toString());
    if (restApi) {
        setUploadedAt(Utils::parseIsoDate(QLatin1String("uploadedAt"), fields));
    } else {
        setUploadedAt(Utils::parseDate(QLatin1String("uploadedAt"), fields));
    }
    setFileId(object.value(QLatin1String("id")).toString());
}

QString File::name() const
{
    return mName;
}

void File::setName(const QString &name)
{
    mName = name;
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
           && (name() == other.name())
           && (url() == other.url())
           && (userId() == other.userId())
           && (mimeType() == other.mimeType())
           && (uploadedAt() == other.uploadedAt())
           && (fileId() == other.fileId())
            && (rid() == other.rid());
}

File &File::operator=(const File &other)
{
    mName = other.name();
    mDescription = other.description();
    mUrl = other.url();
    mUserId = other.userId();
    mMimeType = other.mimeType();
    mUploadedAt = other.uploadedAt();
    mFileId = other.fileId();
    mRid = other.rid();
    return *this;
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

QDebug operator <<(QDebug d, const File &t)
{
    d << "Name : " << t.name();
    d << "Description: " << t.description();
    d << "Url :" << t.url();
    d << "UserId: " << t.userId();
    d << "Mimetype : "<< t.mimeType();
    d << "Uploaded time: " << t.uploadedAt();
    d << "File Id: " << t.fileId();
    d << "Rid: " << t.rid();
    return d;
}
