/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

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
