/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QString>
#include <QTest>

namespace AutoTestHelper
{
void diffFile(const QString &refFile, const QString &generatedFile)
{
    QProcess proc;
#ifdef _WIN32
    QStringList args = QStringList() << QStringLiteral("Compare-Object") << QString(QStringLiteral("(Get-Content %1)")).arg(refFile)
                                     << QString(QStringLiteral("(Get-Content %1)")).arg(generatedFile);

    proc.start(QStringLiteral("powershell"), args);
    QVERIFY(proc.waitForFinished());

    auto pStdOut = proc.readAllStandardOutput();
    if (pStdOut.size()) {
        qDebug() << "Files are different, diff output message:\n" << pStdOut.toStdString().c_str();
    }

    QCOMPARE(pStdOut.size(), 0);
#else
    // compare to reference file
    const QStringList args = QStringList() << QStringLiteral("-u") << refFile << generatedFile;

    proc.setProcessChannelMode(QProcess::ForwardedChannels);
    proc.start(QStringLiteral("diff"), args);
    QVERIFY(proc.waitForFinished());
    QCOMPARE(proc.exitCode(), 0);
#endif
}

void compareFile(const QString &repo, const QByteArray &data, const QString &name)
{
    const QString refFile = QLatin1String(RUQOLA_DATA_DIR) + repo + name + QStringLiteral(".ref");
    const QString generatedFile = QLatin1String(RUQOLA_BINARY_DATA_DIR) + repo + name + QStringLiteral("-generated.ref");
    QDir().mkpath(QLatin1String(RUQOLA_BINARY_DATA_DIR) + repo + name);
    // Create generated file
    QFile f(generatedFile);
    QVERIFY(f.open(QIODevice::WriteOnly | QIODevice::Truncate));
    f.write(data);
    f.close();

    diffFile(refFile, generatedFile);
}

QJsonDocument loadJsonDocument(const QString &originalJsonFile)
{
    QFile f(originalJsonFile);
    [&]() {
        QVERIFY(f.open(QIODevice::ReadOnly));
    }();
    const auto content = f.readAll();
    f.close();

    QJsonParseError error;
    const auto doc = QJsonDocument::fromJson(content, &error);
    [&]() {
        QVERIFY2(!error.error, qPrintable(error.errorString()));
    }();
    return doc;
}

QJsonObject loadJsonObject(const QString &originalJsonFile)
{
    return loadJsonDocument(originalJsonFile).object();
}

QJsonArray loadJsonArrayObject(const QString &originalJsonFile)
{
    return loadJsonDocument(originalJsonFile).array();
}
}
