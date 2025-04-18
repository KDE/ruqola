/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "blocktest.h"
#include "messages/block/block.h"
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
    QCOMPARE(b.blockType(), Block::BlockType::Unknown);

    QVERIFY(!b.videoConferenceInfo().isValid());
    QVERIFY(!b.isValid());
    // Original 432
    QCOMPARE(sizeof(Block), 408);
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
        info.setBlockType(Block::BlockType::VideoConf);
        QTest::addRow("blocks1") << QStringLiteral("blocks1") << info;
    }
    {
        Block info;
        info.setAppId(QStringLiteral("mention-core"));
        info.setBlockType(Block::BlockType::Section);
        info.setSectionText(QStringLiteral("You mentioned *foo*, but they're not in this room."));
        QTest::addRow("section1") << QStringLiteral("section1") << info;
    }
    {
        Block info;
        info.setAppId(QStringLiteral("mention-core"));
        info.setBlockType(Block::BlockType::Actions);

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

    {
        Block info;
        info.setAppId(QStringLiteral("7a8cd36b-5f7b-4177-bd7f-bfc9be908bf8"));
        info.setBlockId(QStringLiteral("366e6622-f43a-11ef-9cd7-3df7975443e9"));
        info.setBlockType(Block::BlockType::Actions);

        QList<BlockAction> lstAct;
        {
            BlockAction act;
            act.setActionId(QStringLiteral("366e3f1a-f43a-11ef-9cd7-3df7975443e9"));
            act.setText(QStringLiteral("Upgrade Account"));
            act.setType(QStringLiteral("button"));
            act.setUrl("https://appsforchat.com/reminder-bot-license"_L1);
            lstAct.append(act);
        }
        {
            BlockAction act;
            act.setActionId(QStringLiteral("366e6620-f43a-11ef-9cd7-3df7975443e9"));
            act.setText(QStringLiteral("More Help"));
            act.setType(QStringLiteral("button"));
            act.setUrl("https://docs.appsforchat.com/reminder-bot"_L1);
            lstAct.append(act);
        }
        {
            BlockAction act;
            act.setActionId(QStringLiteral("366e6621-f43a-11ef-9cd7-3df7975443e9"));
            act.setText(QStringLiteral("contact Us"));
            act.setType(QStringLiteral("button"));
            act.setUrl("https://appsforchat.com/contact"_L1);
            lstAct.append(act);
        }
        info.setBlockActions(lstAct);

        QTest::addRow("action3-url") << QStringLiteral("action3-url") << info;
    }
    {
        Block info;
        info.setAppId(QStringLiteral("c33fa1a6-68a7-491e-bf49-9d7b99671c48"));
        info.setBlockId(QStringLiteral("136c2441-fadf-11ef-85ff-4bacdd8b2d67"));
        info.setBlockType(Block::BlockType::Divider);
        QTest::addRow("divider1") << QStringLiteral("divider1") << info;
    }

    {
        Block info;
        info.setAppId(QStringLiteral("c33fa1a6-68a7-491e-bf49-9d7b99671c48"));
        info.setBlockId(QStringLiteral("136c2440-fadf-11ef-85ff-4bacdd8b2d67"));
        info.setBlockType(Block::BlockType::Section);
        info.setSectionText(QStringLiteral("ssdf"));

        BlockAccessory accessory;
        accessory.setActionId("finish");
        accessory.setType(BlockAccessory::AccessoryType::Overflow);

        QList<BlockAccessoryOption> options;
        BlockAccessoryOption opt;
        opt.setValue(QStringLiteral("finish"));
        opt.setText(QStringLiteral("Finish poll"));
        options.append(opt);
        accessory.setOptions(options);
        info.setBlockAccessory(accessory);
        QTest::addRow("accessory1") << QStringLiteral("accessory1") << info;
    }
    {
        Block info;
        info.setAppId(QStringLiteral("c33fa1a6-68a7-491e-bf49-9d7b99671c48"));
        info.setBlockId(QStringLiteral("d674d766-f513-11ef-82b6-d3aef5c738bf"));
        info.setBlockType(Block::BlockType::Context);

        QList<BlockAction> blockActions;
        BlockAction act;
        act.setActionId("665355b0-4589-4116-8cf2-ba72c4d8c5b8"_L1);
        act.setText("`kkk` 100.00% (1)"_L1);
        act.setType("mrkdwn"_L1);
        blockActions.append(act);
        info.setBlockActions(blockActions);

        QTest::addRow("context1") << QStringLiteral("context1") << info;
    }

    {
        Block info;
        info.setAppId(QStringLiteral("c33fa1a6-68a7-491e-bf49-9d7b99671c48"));
        info.setBlockId(QStringLiteral("33fec462-fcfb-11ef-85ff-4bacdd8b2d67"));
        info.setBlockType(Block::BlockType::Section);
        info.setSectionText("zz"_L1);

        BlockAccessory accessory;
        accessory.setActionId("vote");
        accessory.setValue("0"_L1);
        accessory.setText("Vote"_L1);
        accessory.setType(BlockAccessory::AccessoryType::Button);
        info.setBlockAccessory(std::move(accessory));
        QTest::addRow("section2") << QStringLiteral("section2") << info;
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
    input.setBlockType(Block::BlockType::VideoConf);

    const QJsonObject ba = Block::serialize(input);
    const Block output = Block::deserialize(ba);
    QCOMPARE(input, output);
}

#include "moc_blocktest.cpp"
