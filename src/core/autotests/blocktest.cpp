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
        info.setBlockId(u"63936e304ef3f3baa9658bd7"_s);
        info.setAppId(u"videoconf-core"_s);
        info.setCallId(u"63936e304ef"_s);
        info.setBlockType(Block::BlockType::VideoConf);
        QTest::addRow("blocks1") << u"blocks1"_s << info;
    }
    {
        Block info;
        info.setAppId(u"mention-core"_s);
        info.setBlockType(Block::BlockType::Section);
        info.setSectionText(u"You mentioned *foo*, but they're not in this room."_s);
        QTest::addRow("section1") << u"section1"_s << info;
    }
    {
        Block info;
        info.setAppId(u"mention-core"_s);
        info.setBlockType(Block::BlockType::Actions);

        QList<BlockAction> lstAct;
        {
            BlockAction act;
            act.setActionId(u"add-users"_s);
            act.setText(u"Add them"_s);
            act.setType(u"button"_s);
            act.setBlockId(u"n6bZuzbjfGstE"_s);
            act.setValue(u"[{\"_id\":\"FjvEjYT73XYYo\",\"username\":\"service\",\"name\":\"Service\",\"type\":\"user\"}]"_s);
            lstAct.append(act);
        }
        {
            BlockAction act;
            act.setActionId(u"dismiss"_s);
            act.setText(u"Do nothing"_s);
            act.setType(u"button"_s);
            act.setBlockId(u"n6bZuzbjfGstE"_s);
            act.setValue(u"[{\"_id\":\"HvEjYT73XYYo\",\"username\":\"service\",\"name\":\"Service\",\"type\":\"user\"}]"_s);
            lstAct.append(act);
        }
        {
            BlockAction act;
            act.setActionId(u"share-message"_s);
            act.setText(u"Let them know"_s);
            act.setType(u"button"_s);
            act.setBlockId(u"7bZuzbjfGstE"_s);
            act.setValue(u"[{\"_id\":\"jvEjYT73XYYo\",\"username\":\"service\",\"name\":\"Service\",\"type\":\"user\"}]"_s);
            lstAct.append(act);
        }
        info.setBlockActions(lstAct);
        QTest::addRow("actions1") << u"actions1"_s << info;
    }

    {
        Block info;
        info.setAppId(u"7a8cd36b-5f7b-4177-bd7f-bfc9be908bf8"_s);
        info.setBlockId(u"366e6622-f43a-11ef-9cd7-3df7975443e9"_s);
        info.setBlockType(Block::BlockType::Actions);

        QList<BlockAction> lstAct;
        {
            BlockAction act;
            act.setActionId(u"366e3f1a-f43a-11ef-9cd7-3df7975443e9"_s);
            act.setText(u"Upgrade Account"_s);
            act.setType(u"button"_s);
            act.setUrl("https://appsforchat.com/reminder-bot-license"_L1);
            lstAct.append(act);
        }
        {
            BlockAction act;
            act.setActionId(u"366e6620-f43a-11ef-9cd7-3df7975443e9"_s);
            act.setText(u"More Help"_s);
            act.setType(u"button"_s);
            act.setUrl("https://docs.appsforchat.com/reminder-bot"_L1);
            lstAct.append(act);
        }
        {
            BlockAction act;
            act.setActionId(u"366e6621-f43a-11ef-9cd7-3df7975443e9"_s);
            act.setText(u"contact Us"_s);
            act.setType(u"button"_s);
            act.setUrl("https://appsforchat.com/contact"_L1);
            lstAct.append(act);
        }
        info.setBlockActions(lstAct);

        QTest::addRow("action3-url") << u"action3-url"_s << info;
    }
    {
        Block info;
        info.setAppId(u"c33fa1a6-68a7-491e-bf49-9d7b99671c48"_s);
        info.setBlockId(u"136c2441-fadf-11ef-85ff-4bacdd8b2d67"_s);
        info.setBlockType(Block::BlockType::Divider);
        QTest::addRow("divider1") << u"divider1"_s << info;
    }

    {
        Block info;
        info.setAppId(u"c33fa1a6-68a7-491e-bf49-9d7b99671c48"_s);
        info.setBlockId(u"136c2440-fadf-11ef-85ff-4bacdd8b2d67"_s);
        info.setBlockType(Block::BlockType::Section);
        info.setSectionText(u"ssdf"_s);

        BlockAccessory accessory;
        accessory.setActionId("finish");
        accessory.setType(BlockAccessory::AccessoryType::Overflow);

        QList<BlockAccessoryOption> options;
        BlockAccessoryOption opt;
        opt.setValue(u"finish"_s);
        opt.setText(u"Finish poll"_s);
        options.append(opt);
        accessory.setOptions(options);
        info.setBlockAccessory(accessory);
        QTest::addRow("accessory1") << u"accessory1"_s << info;
    }
    {
        Block info;
        info.setAppId(u"c33fa1a6-68a7-491e-bf49-9d7b99671c48"_s);
        info.setBlockId(u"d674d766-f513-11ef-82b6-d3aef5c738bf"_s);
        info.setBlockType(Block::BlockType::Context);

        QList<BlockAction> blockActions;
        BlockAction act;
        act.setActionId("665355b0-4589-4116-8cf2-ba72c4d8c5b8"_L1);
        act.setText("`kkk` 100.00% (1)"_L1);
        act.setType("mrkdwn"_L1);
        blockActions.append(act);
        info.setBlockActions(blockActions);

        QTest::addRow("context1") << u"context1"_s << info;
    }

    {
        Block info;
        info.setAppId(u"c33fa1a6-68a7-491e-bf49-9d7b99671c48"_s);
        info.setBlockId(u"33fec462-fcfb-11ef-85ff-4bacdd8b2d67"_s);
        info.setBlockType(Block::BlockType::Section);
        info.setSectionText("zz"_L1);

        BlockAccessory accessory;
        accessory.setActionId("vote");
        accessory.setValue("0"_L1);
        accessory.setText("Vote"_L1);
        accessory.setType(BlockAccessory::AccessoryType::Button);
        info.setBlockAccessory(std::move(accessory));
        QTest::addRow("section2") << u"section2"_s << info;
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
    input.setBlockId(u"63936e304ef3f3baa9658bd7"_s);
    input.setAppId(u"videoconf-core"_s);
    input.setCallId(u"63936e304ef"_s);
    input.setBlockType(Block::BlockType::VideoConf);

    const QJsonObject ba = Block::serialize(input);
    const Block output = Block::deserialize(ba);
    QCOMPARE(input, output);
}

#include "moc_blocktest.cpp"
