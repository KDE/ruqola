/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuitest.h"
#include "autogenerateui/autogenerateinteractionui.h"
#include "ruqola_autogenerateui_autotest_helper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>

using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(AutoGenerateInteractionUiTest)

AutoGenerateInteractionUiTest::AutoGenerateInteractionUiTest(QObject *parent)
    : QObject{parent}
{
}

void AutoGenerateInteractionUiTest::shouldHaveDefaultValues()
{
    AutoGenerateInteractionUi ui(nullptr);
    QVERIFY(ui.appId().isEmpty());
    QVERIFY(ui.triggerId().isEmpty());
    QCOMPARE(ui.typeUi(), AutoGenerateInteractionUi::TypeUi::Unknown);
}

void AutoGenerateInteractionUiTest::shouldLoadJson()
{
    QFETCH(QString, fileNameinit);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_AUTOGENERATEUI_DATA_DIR) + "/autogenerateinteractionui/"_L1 + fileNameinit + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    QWidget *parent = new QWidget;
    AutoGenerateInteractionUi ui(nullptr, nullptr);
    QVERIFY(ui.parseInteractionUi(obj));
    QWidget *w = ui.generateWidget(parent);
    QVERIFY(w);

    const QJsonObject objUi = ui.generateJson(true);
    const QJsonDocument docSerialized = QJsonDocument(objUi);
    const QByteArray ba = docSerialized.toJson(QJsonDocument::Indented);
    const QByteArray jsonIndented = docSerialized.toJson(QJsonDocument::Indented);
    AutoTestHelper::compareFile("/autogenerateinteractionui/"_L1, jsonIndented, fileNameinit);
    parent->deleteLater();
    parent = nullptr;
}

void AutoGenerateInteractionUiTest::shouldLoadJson_data()
{
    QTest::addColumn<QString>("fileNameinit");

    QTest::addRow("contextbaropen1") << QStringLiteral("contextbaropen1");
}

#include "moc_autogenerateinteractionuitest.cpp"
