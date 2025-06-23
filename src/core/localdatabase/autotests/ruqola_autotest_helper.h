/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDebug>
using namespace Qt::Literals::StringLiterals;

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
    QStringList args = QStringList() << u"Compare-Object"_s << QString(u"(Get-Content %1)"_s).arg(refFile) << QString(u"(Get-Content %1)"_s).arg(generatedFile);

    proc.start(u"powershell"_s, args);
    QVERIFY(proc.waitForFinished());

    auto pStdOut = proc.readAllStandardOutput();
    if (pStdOut.size()) {
        qDebug() << "Files are different, diff output message:\n" << pStdOut.toStdString().c_str();
    }

    QCOMPARE(pStdOut.size(), 0);
#else
    // compare to reference file
    const QStringList args = QStringList() << u"-u"_s << refFile << generatedFile;

    proc.setProcessChannelMode(QProcess::ForwardedChannels);
    proc.start(u"diff"_s, args);
    QVERIFY(proc.waitForFinished());
    QCOMPARE(proc.exitCode(), 0);
#endif
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
