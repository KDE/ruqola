/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewinputblocktest.h"
#include "autogenerateui/blockelement/autogenerateinteractionuiviewinputblock.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewplaintextinputelement.h"
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
    const AutoGenerateInteractionUiViewInputBlock b;
    QVERIFY(!b.optional());
    QVERIFY(!b.element());
}

void AutoGenerateInteractionUiViewInputBlockTest::shouldLoadJson()
{
    QFETCH(QString, name);
    QFETCH(AutoGenerateInteractionUiViewInputBlock *, inputElement);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_AUTOGENERATEUI_DATA_DIR) + "/autogenerateinteractionuiviewinputblock/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    AutoGenerateInteractionUiViewInputBlock result;
    result.parse(obj);
    const bool equal = result == *inputElement;
    if (!equal) {
        qDebug() << " result " << result;
        qDebug() << " inputElement " << *inputElement;
    }
    delete inputElement;
    QVERIFY(equal);
}

void AutoGenerateInteractionUiViewInputBlockTest::shouldLoadJson_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<AutoGenerateInteractionUiViewInputBlock *>("inputElement");

    {
        AutoGenerateInteractionUiViewInputBlock *textElement = new AutoGenerateInteractionUiViewInputBlock;
        textElement->setBlockId("autoReplySettings"_ba);
        textElement->setAppId("821cd5c6-1fb5-4d9e-8e88-e6176463efb6"_ba);
        textElement->setType("input"_ba);
        AutoGenerateInteractionUiViewText text;
        text.setText("Auto-reply Message:"_L1);
        text.setType(AutoGenerateInteractionUiViewText::TextType::PlainText);
        textElement->setLabel(text);
        auto element = new AutoGenerateInteractionUiViewPlainTextInputElement;
        element->setType("plain_text_input"_ba);
        element->setActionId("AutoReplyMessage"_ba);
        element->setInitialValue("Hey, I received your message and will get back to you as soon as possible."_L1);
        element->setMultiLine(true);
        textElement->setElement(element);
        QTest::addRow("test1") << u"test1"_s << textElement;
    }
}

#include "moc_autogenerateinteractionuiviewinputblocktest.cpp"
