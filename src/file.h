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
#include <QObject>
#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_TESTS_EXPORT File
{
    Q_GADGET
public:
    File();
    ~File();

    void parseFile(const QJsonObject &json);
    QString name() const;
    void setName(const QString &name);

    QString description() const;
    void setDescription(const QString &description);

    bool operator ==(const File &other) const;

    File &operator=(const File &other);
    QString userId() const;
    void setUserId(const QString &userId);

    QString url() const;
    void setUrl(const QString &url);

    QString mimeType() const;
    void setMimeType(const QString &mimeType);

    qint64 uploadedAt() const;
    void setUploadedAt(const qint64 &uploadedAt);

private:
    QString mName;
    QString mDescription;
    QString mUserId;
    QString mUrl;
    QString mMimeType;
    qint64 mUploadedAt = -1;
};
Q_DECLARE_METATYPE(File)
Q_DECLARE_TYPEINFO(File, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const File &t);

#endif // FILES_H
