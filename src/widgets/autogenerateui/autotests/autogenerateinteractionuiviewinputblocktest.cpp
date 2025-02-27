/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewinputblocktest.h"
#include "autogenerateui/blockelement/autogenerateinteractionuiviewinputblock.h"
#include "ruqola_autogenerateui_autotest_helper.h"
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(AutoGenerateInteractionUiViewInputBlockTest)
AutoGenerateInteractionUiViewInputBlockTest::AutoGenerateInteractionUiViewInputBlockTest(QObject *parent)
    : QObject{parent}
{
}

void AutoGenerateInteractionUiViewInputBlockTest::shouldHaveDefaultValues()
{
    AutoGenerateInteractionUiViewInputBlock b;
    QVERIFY(!b.optional());
    QVERIFY(!b.element());
}

void AutoGenerateInteractionUiViewInputBlockTest::shouldLoadJson()
{
    QFETCH(QString, name);
    QFETCH(AutoGenerateInteractionUiViewInputBlock, inputElement);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_AUTOGENERATEUI_DATA_DIR) + "/autogenerateinteractionuiviewinputblock/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    AutoGenerateInteractionUiViewInputBlock result;
    result.parse(obj);
    const bool equal = result == inputElement;
    if (!equal) {
        qDebug() << " result " << result;
        qDebug() << " inputElement " << inputElement;
    }
    QVERIFY(equal);
}

void AutoGenerateInteractionUiViewInputBlockTest::shouldLoadJson_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<AutoGenerateInteractionUiViewInputBlock>("inputElement");

    {
        AutoGenerateInteractionUiViewInputBlock textElement;
        textElement.setBlockId("autoReplySettings"_L1);
        textElement.setAppId("821cd5c6-1fb5-4d9e-8e88-e6176463efb6"_L1);
        textElement.setType("input"_L1);
        AutoGenerateInteractionUiViewText text;
        text.setText("Auto-reply Message:"_L1);
        text.setType(AutoGenerateInteractionUiViewText::TextType::PlainText);
        textElement.setLabel(text);
        QTest::addRow("test1") << QStringLiteral("test1") << textElement;
    }
}

#include "moc_autogenerateinteractionuiviewinputblocktest.cpp"
