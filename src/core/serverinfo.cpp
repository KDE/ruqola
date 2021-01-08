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

#include "serverinfo.h"

ServerInfo::ServerInfo()
{
}

ServerInfo::~ServerInfo()
{
}

void ServerInfo::parseServerInfo(const QJsonObject &obj)
{
    const QJsonObject infoObj = obj[QLatin1String("info")].toObject();
    mVersion = infoObj.value(QLatin1String("version")).toString();
    const QJsonObject commitObj = infoObj[QLatin1String("commit")].toObject();
    mCommitAuthor = commitObj.value(QLatin1String("author")).toString();
    mCommitBranch = commitObj.value(QLatin1String("branch")).toString();
    mCommitTag = commitObj.value(QLatin1String("tag")).toString();
    mCommitSubject = commitObj.value(QLatin1String("subject")).toString();
    mCommitHash = commitObj.value(QLatin1String("hash")).toString();

    const QJsonObject buildObj = infoObj[QLatin1String("build")].toObject();
    mArch = buildObj.value(QLatin1String("arch")).toString();
    mNumberOfCpu = buildObj.value(QLatin1String("cpus")).toInt();
    mPlatform = buildObj.value(QLatin1String("platform")).toString();
#if 0
    {
        "info" : {"build" : {"arch" : "x64", "cpus" : 2, "date" : "2020-12-29T04:59:35.728Z", "freeMemory" : 461508608, "nodeVersion" : "v12.18.4", "osRelease" : "5.4.0-1032-azure", "platform" : "linux", "totalMemory" : 7292207104},
                  "commit" : {"author" : "Diego Sampaio", "branch" : "HEAD", "date" : "Tue Dec 29 01:43:25 2020 -0300", "hash" : "3a13cead22bfc1100c5b89069498919473c84195", "subject" : "Merge pull request #19982 from RocketChat/release-3.10.0", "tag" : "3.10.0"},
                  "marketplaceApiVersion" : "1.21.0-alpha.4235", "version" : "3.10.0"}, "success" : true
    }
    //TODO
#endif
}

QString ServerInfo::arch() const
{
    return mArch;
}

void ServerInfo::setArch(const QString &arch)
{
    mArch = arch;
}

int ServerInfo::numberOfCpu() const
{
    return mNumberOfCpu;
}

void ServerInfo::setNumberOfCpu(int numberOfCpu)
{
    mNumberOfCpu = numberOfCpu;
}

QString ServerInfo::platform() const
{
    return mPlatform;
}

void ServerInfo::setPlatform(const QString &platform)
{
    mPlatform = platform;
}

QString ServerInfo::version() const
{
    return mVersion;
}

void ServerInfo::setVersion(const QString &version)
{
    mVersion = version;
}

QString ServerInfo::commitAuthor() const
{
    return mCommitAuthor;
}

void ServerInfo::setCommitAuthor(const QString &commitAuthor)
{
    mCommitAuthor = commitAuthor;
}

QString ServerInfo::commitBranch() const
{
    return mCommitBranch;
}

void ServerInfo::setCommitBranch(const QString &commitBranch)
{
    mCommitBranch = commitBranch;
}

QString ServerInfo::commitTag() const
{
    return mCommitTag;
}

void ServerInfo::setCommitTag(const QString &commitTag)
{
    mCommitTag = commitTag;
}

QString ServerInfo::commitSubject() const
{
    return mCommitSubject;
}

void ServerInfo::setCommitSubject(const QString &commitSubject)
{
    mCommitSubject = commitSubject;
}

QString ServerInfo::commitHash() const
{
    return mCommitHash;
}

void ServerInfo::setCommitHash(const QString &commitHash)
{
    mCommitHash = commitHash;
}

QDebug operator <<(QDebug d, const ServerInfo &t)
{
    d << "arch " << t.arch();
    d << "platform " << t.platform();
    d << "version " << t.version();
    d << "cpu number " << t.numberOfCpu();
    d << "commit author " << t.commitAuthor();
    d << "commit branch " << t.commitBranch();
    d << "commit tag " << t.commitTag();
    d << "commit subject " << t.commitSubject();
    d << "commit hash " << t.commitHash();
    return d;
}
