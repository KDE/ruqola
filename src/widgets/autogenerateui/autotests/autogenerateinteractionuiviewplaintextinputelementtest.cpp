/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewplaintextinputelementtest.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewplaintextinputelement.h"
#include "ruqola_autogenerateui_autotest_helper.h"
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(AutoGenerateInteractionUiViewPlainTextInputElementTest)

AutoGenerateInteractionUiViewPlainTextInputElementTest::AutoGenerateInteractionUiViewPlainTextInputElementTest(QObject *parent)
    : QObject{parent}
{
}

void AutoGenerateInteractionUiViewPlainTextInputElementTest::shouldHaveDefaultValues()
{
    AutoGenerateInteractionUiViewPlainTextInputElement e;
    QVERIFY(!e.placeHolder());
    QVERIFY(e.initialValue().isEmpty());
    QVERIFY(!e.multiLine());
    QCOMPARE(e.minLength(), -1);
    QCOMPARE(e.maxLength(), -1);
}

void AutoGenerateInteractionUiViewPlainTextInputElementTest::shouldLoadJson()
{
    QFETCH(QString, name);
    QFETCH(AutoGenerateInteractionUiViewPlainTextInputElement *, inputElement);
    const QString originalJsonFile =
        QLatin1StringView(RUQOLA_AUTOGENERATEUI_DATA_DIR) + "/autogenerateinteractionuiviewplaintextinputelement/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    AutoGenerateInteractionUiViewPlainTextInputElement result;
    result.parse(obj);
    const bool equal = result == *inputElement;
    if (!equal) {
        qDebug() << " result " << result;
        qDebug() << " inputElement " << inputElement;
    }
    delete inputElement;
    QVERIFY(equal);
}

void AutoGenerateInteractionUiViewPlainTextInputElementTest::shouldLoadJson_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<AutoGenerateInteractionUiViewPlainTextInputElement *>("inputElement");

    {
        AutoGenerateInteractionUiViewPlainTextInputElement *textElement = new AutoGenerateInteractionUiViewPlainTextInputElement;
        textElement->setInitialValue("Hey, I received your message and will get back to you as soon as possible."_L1);
        textElement->setMultiLine(true);
        textElement->setType("plain_text_input");
        textElement->setActionId("AutoReplyMessage");
        QTest::addRow("plaintextinput") << u"plaintextinput"_s << textElement;
    }
}

#include "moc_autogenerateinteractionuiviewplaintextinputelementtest.cpp"
