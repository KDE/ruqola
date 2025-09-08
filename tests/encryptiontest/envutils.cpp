/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "envutils.h"
#include <QFile>

QHash<QString, QString> loadEnvFile(const QString &filePath)
{
    QHash<QString, QString> env;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return env;
    }
    while (!file.atEnd()) {
        const QString line = QString::fromUtf8(file.readLine()).trimmed();
        if (line.startsWith(QLatin1Char('#')) || line.isEmpty())
            continue;
        const int equalSignIndex = line.indexOf(QLatin1Char('='));
        if (equalSignIndex == -1)
            continue;
        const QString key = line.left(equalSignIndex).trimmed();
        const QString value = line.mid(equalSignIndex + 1).trimmed();
        env[key] = value;
    }
    return env;
}
