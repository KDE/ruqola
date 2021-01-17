/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#include "searchchannelfilterproxymodeltest.h"

#include "model/searchchannelfilterproxymodel.h"
#include "model/searchchannelmodel.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>

QTEST_GUILESS_MAIN(SearchChannelFilterProxyModelTest)

SearchChannelFilterProxyModelTest::SearchChannelFilterProxyModelTest(QObject *parent)
    : QObject(parent)
{
}

void SearchChannelFilterProxyModelTest::shouldHaveDefaultValue()
{
    SearchChannelFilterProxyModel w;
    SearchChannelModel sourceModel;
    w.setSourceModel(&sourceModel);
    QCOMPARE(w.rowCount(), 0);
    QVERIFY(w.sourceModel());
    QCOMPARE(w.sourceModel(), &sourceModel);
}

QJsonObject loadFile(const QString &file)
{
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/") + file;
    QFile f(originalJsonFile);
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning() << " Unable to load file " << file;
        return {};
    }
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject root = doc.object();
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    return obj;
}

void SearchChannelFilterProxyModelTest::shouldLoadChannelJSon()
{
    SearchChannelFilterProxyModel w;
    SearchChannelModel sourceModel;
    w.setSourceModel(&sourceModel);

    QJsonObject obj = loadFile(QStringLiteral("channelparent.json"));
    sourceModel.parseChannels(obj);
    QCOMPARE(w.rowCount(), 8);

    //TODO improve
}
