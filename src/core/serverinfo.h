/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef SERVERINFO_H
#define SERVERINFO_H

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QString>

class LIBRUQOLACORE_EXPORT ServerInfo
{
public:
    ServerInfo();
    ~ServerInfo();

    void parseServerInfo(const QJsonObject &obj);
    Q_REQUIRED_RESULT QString arch() const;
    void setArch(const QString &arch);

    Q_REQUIRED_RESULT int numberOfCpu() const;
    void setNumberOfCpu(int numberOfCpu);

    Q_REQUIRED_RESULT QString platform() const;
    void setPlatform(const QString &platform);

    Q_REQUIRED_RESULT QString version() const;
    void setVersion(const QString &version);

    Q_REQUIRED_RESULT QString commitAuthor() const;
    void setCommitAuthor(const QString &commitAuthor);

    Q_REQUIRED_RESULT QString commitBranch() const;
    void setCommitBranch(const QString &commitBranch);

    Q_REQUIRED_RESULT QString commitTag() const;
    void setCommitTag(const QString &commitTag);

    Q_REQUIRED_RESULT QString commitSubject() const;
    void setCommitSubject(const QString &commitSubject);

    Q_REQUIRED_RESULT QString commitHash() const;
    void setCommitHash(const QString &commitHash);

    Q_REQUIRED_RESULT bool operator==(const ServerInfo &other) const;

    Q_REQUIRED_RESULT QString osRelease() const;
    void setOsRelease(const QString &osRelease);

    Q_REQUIRED_RESULT QString nodeVersion() const;
    void setNodeVersion(const QString &nodeVersion);

private:
    QString mArch;
    QString mPlatform;
    QString mVersion;
    QString mCommitAuthor;
    QString mCommitBranch;
    QString mCommitTag;
    QString mCommitSubject;
    QString mCommitHash;
    QString mOsRelease;
    QString mNodeVersion;
    // TODO Build Date
    int mNumberOfCpu = -1;
};
Q_DECLARE_METATYPE(ServerInfo)
Q_DECLARE_TYPEINFO(ServerInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ServerInfo &t);

#endif // SERVERINFO_H
