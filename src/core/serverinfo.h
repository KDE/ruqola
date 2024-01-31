/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

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
    [[nodiscard]] QString arch() const;
    void setArch(const QString &arch);

    [[nodiscard]] int numberOfCpu() const;
    void setNumberOfCpu(int numberOfCpu);

    [[nodiscard]] QString platform() const;
    void setPlatform(const QString &platform);

    [[nodiscard]] QString version() const;
    void setVersion(const QString &version);

    [[nodiscard]] QString commitAuthor() const;
    void setCommitAuthor(const QString &commitAuthor);

    [[nodiscard]] QString commitBranch() const;
    void setCommitBranch(const QString &commitBranch);

    [[nodiscard]] QString commitTag() const;
    void setCommitTag(const QString &commitTag);

    [[nodiscard]] QString commitSubject() const;
    void setCommitSubject(const QString &commitSubject);

    [[nodiscard]] QString commitHash() const;
    void setCommitHash(const QString &commitHash);

    [[nodiscard]] bool operator==(const ServerInfo &other) const;

    [[nodiscard]] QString osRelease() const;
    void setOsRelease(const QString &osRelease);

    [[nodiscard]] QString nodeVersion() const;
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
Q_DECLARE_TYPEINFO(ServerInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ServerInfo &t);
