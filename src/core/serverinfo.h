/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

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
