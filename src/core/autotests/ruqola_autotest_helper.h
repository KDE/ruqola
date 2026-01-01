/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QString>
class QJsonDocument;
class QJsonObject;
class QJsonArray;
namespace AutoTestHelper
{
void diffFile(const QString &refFile, const QString &generatedFile);
void compareFile(const QString &repo, const QByteArray &data, const QString &name);
QJsonDocument loadJsonDocument(const QString &originalJsonFile);
QJsonObject loadJsonObject(const QString &originalJsonFile);
QJsonArray loadJsonArrayObject(const QString &originalJsonFile);
}
