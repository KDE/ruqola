/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverinfotest.h"
#include "ruqola_autotest_helper.h"
#include "serverinfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(ServerInfoTest)

ServerInfoTest::ServerInfoTest(QObject *parent)
    : QObject(parent)
{
}

void ServerInfoTest::shouldHaveDefaultValues()
{
    ServerInfo w;
    QVERIFY(w.arch().isEmpty());
    QVERIFY(w.commitAuthor().isEmpty());
    QVERIFY(w.platform().isEmpty());
    QVERIFY(w.version().isEmpty());
    QCOMPARE(w.numberOfCpu(), -1);

    QVERIFY(w.commitAuthor().isEmpty());
    QVERIFY(w.commitBranch().isEmpty());
    QVERIFY(w.commitTag().isEmpty());
    QVERIFY(w.commitSubject().isEmpty());
    QVERIFY(w.commitHash().isEmpty());

    QVERIFY(w.osRelease().isEmpty());
    QVERIFY(w.nodeVersion().isEmpty());
    // TODO
}

void ServerInfoTest::shouldLoadServerInfo_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<ServerInfo>("serverInfo");
    {
        ServerInfo info;
        info.setNumberOfCpu(2);
        info.setArch(QStringLiteral("x64"));
        info.setPlatform(QStringLiteral("linux"));
        info.setVersion(QStringLiteral("3.10.0"));
        info.setCommitAuthor(QStringLiteral("Diego Sampaio"));
        info.setCommitBranch(QStringLiteral("HEAD"));
        info.setCommitTag(QStringLiteral("3.10.0"));
        info.setCommitSubject(QStringLiteral("Merge pull request #19982 from RocketChat/release-3.10.0"));
        info.setCommitHash(QStringLiteral("3a13cead22bfc1100c5b89069498919473c84195"));
        info.setOsRelease(QStringLiteral("5.4.0-1032-azure"));
        info.setNodeVersion(QStringLiteral("v12.18.4"));

        QTest::addRow("default") << QStringLiteral("serverinfo") << info;
    }
}

void ServerInfoTest::shouldLoadServerInfo()
{
    QFETCH(QString, fileName);
    QFETCH(ServerInfo, serverInfo);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/serverinfo/") + fileName + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    ServerInfo r;
    r.parseServerInfo(obj);
    const bool result = (r == serverInfo);
    if (!result) {
        qDebug() << "result " << r;
        qDebug() << "expected " << serverInfo;
    }
    QVERIFY(result);
}

#include "moc_serverinfotest.cpp"
