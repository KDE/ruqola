/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

    // TODO improve
}

#include "moc_searchchannelfilterproxymodeltest.cpp"
