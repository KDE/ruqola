/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewtexttest.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "ruqola_autogenerateui_autotest_helper.h"
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(AutoGenerateInteractionUiViewTextTest)

AutoGenerateInteractionUiViewTextTest::AutoGenerateInteractionUiViewTextTest(QObject *parent)
    : QObject{parent}
{
}

void AutoGenerateInteractionUiViewTextTest::shouldHaveDefaultValues()
{
    AutoGenerateInteractionUiViewText t;
    QVERIFY(t.text().isEmpty());
    QVERIFY(!t.emoji());
    QCOMPARE(t.type(), AutoGenerateInteractionUiViewText::TextType::Unknown);
}

void AutoGenerateInteractionUiViewTextTest::shouldLoadJson()
{
    QFETCH(QString, name);
    QFETCH(AutoGenerateInteractionUiViewText, textelement);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_AUTOGENERATEUI_DATA_DIR) + "/autogenerateinteractionuiviewtext/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    AutoGenerateInteractionUiViewText result;
    result.parse(obj);
    const bool equal = result == textelement;
    if (!equal) {
        qDebug() << " result " << result;
        qDebug() << " textelement " << textelement;
    }
    QVERIFY(equal);
}

void AutoGenerateInteractionUiViewTextTest::shouldLoadJson_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<AutoGenerateInteractionUiViewText>("textelement");

    {
        AutoGenerateInteractionUiViewText textElement;
        textElement.setText("**Past and Incomplete:**"_L1);
        textElement.setEmoji(false);
        textElement.setType(AutoGenerateInteractionUiViewText::TextType::Markdown);

        QTest::addRow("mardownelement") << u"mardownelement"_s << textElement;
    }
    {
        AutoGenerateInteractionUiViewText textElement;
        textElement.setText("Close"_L1);
        textElement.setEmoji(false);
        textElement.setType(AutoGenerateInteractionUiViewText::TextType::PlainText);

        QTest::addRow("plaintextelement") << u"plaintextelement"_s << textElement;
    }
}

#include "moc_autogenerateinteractionuiviewtexttest.cpp"
