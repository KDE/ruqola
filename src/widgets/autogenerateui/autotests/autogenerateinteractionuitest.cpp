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
    QFETCH(QByteArray, serializeState);

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
    const QByteArray jsonIndented = ba;
    QCOMPARE(QJsonDocument(ui.serializeState()).toJson(), serializeState);

    AutoTestHelper::compareFile("/autogenerateinteractionui/"_L1, jsonIndented, fileNameinit);
    parent->deleteLater();
    parent = nullptr;
}

void AutoGenerateInteractionUiTest::shouldLoadJson_data()
{
    QTest::addColumn<QString>("fileNameinit");
    QTest::addColumn<QByteArray>("serializeState");

    QTest::addRow("contextbaropen1") << u"contextbaropen1"_s
                                     << QByteArray(
                                            "{\n    \"autoReplySettings\": {\n        \"AutoReplyMessage\": \"Hey, I received your message and will get back "
                                            "to you as soon as possible.\"\n    }\n}\n");
    QTest::addRow("modalupdate1")
        << u"modalupdate1"_s
        << QByteArray(
               "{\n    \"reminderData\": {\n        \"message\": \"\",\n        \"repeat\": \"once\",\n        \"targetChannel\": \"\",\n        "
               "\"targetType\": \"channel\",\n        \"whenDate\": \"18/03/2025\",\n        \"whenTime\": \"14:00\"\n    }\n}\n");

    QTest::addRow("datetime")
        << u"datetime"_s
        << QByteArray(
               "{\n    \"43ac4271-0584-11f0-9f18-1735ff3ce6ab\": {\n        "
               "\"linked-msg-reminder-change-duration-option_msgReminderCreateModal-0c4e4b86-6de8-415e-95e6-de42643f3bcb\": \"custom\"\n    },\n    "
               "\"reminder\": {\n        \"date\": \"2025-03-20\",\n        \"description\": \"\",\n        \"time\": \"01:15 PM\"\n    }\n}\n");
}

#include "moc_autogenerateinteractionuitest.cpp"
