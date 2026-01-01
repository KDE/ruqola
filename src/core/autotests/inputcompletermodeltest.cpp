/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inputcompletermodeltest.h"

#include "model/inputcompletermodel.h"
#include "test_model_helpers.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>

QTEST_GUILESS_MAIN(InputCompleterModelTest)

using namespace Qt::Literals::StringLiterals;
InputCompleterModelTest::InputCompleterModelTest(QObject *parent)
    : QObject(parent)
{
}

void InputCompleterModelTest::shouldHaveDefaultValue()
{
    InputCompleterModel w(nullptr);
    QCOMPARE(w.rowCount(), 0);
}

void InputCompleterModelTest::shouldAssignValues()
{
    InputCompleterModel w(nullptr);
    QSignalSpy rowInsertedSpy(&w, &InputCompleterModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &InputCompleterModel::rowsAboutToBeInserted);
    QSignalSpy modelAboutToResetSpy(&w, &InputCompleterModel::modelAboutToBeReset);

    QList<ChannelUserCompleter> channelList;
    for (int i = 0; i < 10; ++i) {
        ChannelUserCompleter c;
        c.setName(u"roomname%1"_s.arg(i));
        channelList.append(std::move(c));
    }
    w.setChannels(channelList);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(modelAboutToResetSpy.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), u"0,9"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), u"0,9"_s);

    // add Empty list
    channelList.clear();
    rowInsertedSpy.clear();
    rowABTInserted.clear();
    modelAboutToResetSpy.clear();

    w.setChannels(channelList);

    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(modelAboutToResetSpy.count(), 1);

    // Add same element
    rowInsertedSpy.clear();
    rowABTInserted.clear();
    modelAboutToResetSpy.clear();

    w.setChannels(channelList);

    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(modelAboutToResetSpy.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QString());
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QString());

    // Test add same number of element.
    channelList.clear();
    rowInsertedSpy.clear();
    rowABTInserted.clear();
    modelAboutToResetSpy.clear();

    for (int i = 0; i < 5; ++i) {
        ChannelUserCompleter c;
        c.setName(u"roomname%1"_s.arg(i));
        channelList.append(std::move(c));
    }
    w.setChannels(channelList);
    QCOMPARE(w.rowCount(), 5);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(modelAboutToResetSpy.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), u"0,4"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), u"0,4"_s);

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    modelAboutToResetSpy.clear();
    w.setChannels(channelList);

    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(modelAboutToResetSpy.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), u"0,4"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), u"0,4"_s);
}

static QJsonObject loadFile(const QString &file)
{
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/"_L1 + file;
    QFile f(originalJsonFile);
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning() << " Unable to load file " << file;
        return {};
    }
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject root = doc.object();
    const QJsonObject obj = root.value("result"_L1).toObject();
    return obj;
}

void InputCompleterModelTest::shouldLoadValueFromJson()
{
    InputCompleterModel w(nullptr);
    QSignalSpy rowInsertedSpy(&w, &InputCompleterModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &InputCompleterModel::rowsAboutToBeInserted);
    QSignalSpy modelAboutToResetSpy(&w, &InputCompleterModel::modelAboutToBeReset);

    QJsonObject obj = loadFile(u"channelparent.json"_s);
    InputCompleterModel::SearchInfo info;
    info.searchType = InputCompleterModel::SearchInfo::ChannelsAndUsers;
    w.setSearchInfo(std::move(info));
    w.parseChannels(obj);
    QCOMPARE(w.rowCount(), 8);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(modelAboutToResetSpy.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), u"0,7"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), u"0,7"_s);

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    modelAboutToResetSpy.clear();

    // Test room
    QCOMPARE(w.data(w.index(2), InputCompleterModel::CompleterName).toString(), u"bal3"_s);
    QCOMPARE(w.data(w.index(2), InputCompleterModel::ChannelType).value<ChannelUserCompleter::ChannelUserCompleterType>(),
             ChannelUserCompleter::ChannelUserCompleterType::Room);
    QCOMPARE(w.data(w.index(2), InputCompleterModel::DisplayName).toString(), u"bal3"_s);

    // Test user
    QCOMPARE(w.data(w.index(6), InputCompleterModel::CompleterName).toString(), u"bla.foo4"_s);
    QCOMPARE(w.data(w.index(6), InputCompleterModel::ChannelType).value<ChannelUserCompleter::ChannelUserCompleterType>(),
             ChannelUserCompleter::ChannelUserCompleterType::DirectChannel);
    // We use for user a channelid == channel name as we use it for opening direct channel
    QCOMPARE(w.data(w.index(6), InputCompleterModel::DisplayName).toString(), u"foo4"_s);

    // Test without name/username !

    obj = loadFile(u"channelparentempty.json"_s);
    w.parseChannels(obj);
    QCOMPARE(w.rowCount(), 1); // "No found result" item
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(modelAboutToResetSpy.count(), 1);
}

void InputCompleterModelTest::shouldClearModel()
{
    InputCompleterModel w(nullptr);
    QSignalSpy rowInsertedSpy(&w, &InputCompleterModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &InputCompleterModel::rowsAboutToBeInserted);
    QSignalSpy modelAboutToBeResetSpy(&w, &InputCompleterModel::modelAboutToBeReset);

    QJsonObject obj = loadFile(u"channelparent.json"_s);
    InputCompleterModel::SearchInfo info;
    info.searchType = InputCompleterModel::SearchInfo::ChannelsAndUsers;
    w.setSearchInfo(std::move(info));
    w.parseChannels(obj);
    QCOMPARE(w.rowCount(), 8);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(modelAboutToBeResetSpy.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), u"0,7"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), u"0,7"_s);

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    modelAboutToBeResetSpy.clear();

    w.clear();
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(modelAboutToBeResetSpy.count(), 1);
}

#include "moc_inputcompletermodeltest.cpp"
