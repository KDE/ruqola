/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "blocktest.h"
#include "messages/block.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(BlockTest)
BlockTest::BlockTest(QObject *parent)
    : QObject{parent}
{
}

void BlockTest::shouldHaveDefaultValues()
{
    Block b;
    QVERIFY(b.callId().isEmpty());
    QVERIFY(b.blockId().isEmpty());
    QVERIFY(b.appId().isEmpty());
    QVERIFY(b.blockTypeStr().isEmpty());
    QCOMPARE(b.blockType(), Block::Unknown);

    QVERIFY(!b.videoConferenceInfo().isValid());
    QVERIFY(!b.isValid());
}

void BlockTest::shouldLoadBlock_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<Block>("blockInfo");
    {
        Block info;
        info.setBlockId(QStringLiteral("63936e304ef3f3baa9658bd7"));
        info.setAppId(QStringLiteral("videoconf-core"));
        info.setCallId(QStringLiteral("63936e304ef"));
        info.setBlockTypeStr(QStringLiteral("video_conf"));
        QTest::addRow("blocks1") << QStringLiteral("blocks1") << info;
    }
    {
        Block info;
        info.setAppId(QStringLiteral("mention-core"));
        info.setBlockTypeStr(QStringLiteral("section"));
        info.setSectionText(QStringLiteral("You mentioned *foo*, but they're not in this room."));
        QTest::addRow("section1") << QStringLiteral("section1") << info;
    }
    {
        Block info;
        info.setAppId(QStringLiteral("mention-core"));
        info.setBlockTypeStr(QStringLiteral("actions"));

        QList<BlockAction> lstAct;
        {
            BlockAction act;
            act.setActionId(QStringLiteral("add-users"));
            act.setText(QStringLiteral("Add them"));
            act.setType(QStringLiteral("button"));
            act.setBlockId(QStringLiteral("n6bZuzbjfGstE"));
            act.setValue(QStringLiteral("[{\"_id\":\"FjvEjYT73XYYo\",\"username\":\"service\",\"name\":\"Service\",\"type\":\"user\"}]"));
            lstAct.append(act);
        }
        {
            BlockAction act;
            act.setActionId(QStringLiteral("dismiss"));
            act.setText(QStringLiteral("Do nothing"));
            act.setType(QStringLiteral("button"));
            act.setBlockId(QStringLiteral("n6bZuzbjfGstE"));
            act.setValue(QStringLiteral("[{\"_id\":\"HvEjYT73XYYo\",\"username\":\"service\",\"name\":\"Service\",\"type\":\"user\"}]"));
            lstAct.append(act);
        }
        {
            BlockAction act;
            act.setActionId(QStringLiteral("share-message"));
            act.setText(QStringLiteral("Let them know"));
            act.setType(QStringLiteral("button"));
            act.setBlockId(QStringLiteral("7bZuzbjfGstE"));
            act.setValue(QStringLiteral("[{\"_id\":\"jvEjYT73XYYo\",\"username\":\"service\",\"name\":\"Service\",\"type\":\"user\"}]"));
            lstAct.append(act);
        }
        info.setBlockActions(lstAct);
        QTest::addRow("actions1") << QStringLiteral("actions1") << info;
    }
}

void BlockTest::shouldLoadBlock()
{
    QFETCH(QString, name);
    QFETCH(Block, blockInfo);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/blocks/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    Block r;
    r.parseBlock(obj);
    const bool equalOwner = (r == blockInfo);
    if (!equalOwner) {
        qDebug() << "ACTUAL " << r;
        qDebug() << "EXPECTED " << blockInfo;
    }
    QVERIFY(equalOwner);
}

void BlockTest::shouldSerializeData()
{
    Block input;
    input.setBlockId(QStringLiteral("63936e304ef3f3baa9658bd7"));
    input.setAppId(QStringLiteral("videoconf-core"));
    input.setCallId(QStringLiteral("63936e304ef"));
    input.setBlockTypeStr(QStringLiteral("video_conf"));

    const QJsonObject ba = Block::serialize(input);
    const Block output = Block::deserialize(ba);
    QCOMPARE(input, output);
}

#include "moc_blocktest.cpp"
