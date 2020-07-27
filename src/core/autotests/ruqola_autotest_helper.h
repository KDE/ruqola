/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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

#ifndef RUQOLA_AUTOTEST_HELPER_H
#define RUQOLA_AUTOTEST_HELPER_H

#include <QProcess>
#include <QString>
#include <QTest>
#include <QDebug>

namespace AutoTestHelper {
void diffFile(const QString &refFile, const QString &generatedFile)
{
    QProcess proc;
#ifdef _WIN32
    QStringList args = QStringList()
                       << QStringLiteral("Compare-Object")
                       << QString(QStringLiteral("(Get-Content %1)")).arg(refFile)
                       << QString(QStringLiteral("(Get-Content %1)")).arg(generatedFile);

    proc.start(QStringLiteral("powershell"), args);
    QVERIFY(proc.waitForFinished());

    auto pStdOut = proc.readAllStandardOutput();
    if (pStdOut.size()) {
        qDebug() << "Files are different, diff output message:\n"
                 << pStdOut.toStdString().c_str();
    }

    QCOMPARE(pStdOut.size(), 0);
#else
    // compare to reference file
    QStringList args = QStringList()
                       << QStringLiteral("-u")
                       << refFile
                       << generatedFile;

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
    //Create generated file
    QFile f(generatedFile);
    QVERIFY(f.open(QIODevice::WriteOnly | QIODevice::Truncate));
    f.write(data);
    f.close();

    diffFile(refFile, generatedFile);
}
}

#endif // RUQOLA_AUTOTEST_HELPER_H
