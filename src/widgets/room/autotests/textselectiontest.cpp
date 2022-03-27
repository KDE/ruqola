/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textselectiontest.h"
#include "model/messagemodel.h"
#include "room/delegate/textselection.h"

#include <QSignalSpy>
#include <QTest>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextDocumentFragment>

QTEST_MAIN(TextSelectionTest)

TextSelectionTest::TextSelectionTest(QObject *parent)
    : QObject(parent)
{
}

class TestFactory : public DocumentFactoryInterface
{
public:
    explicit TestFactory(int numRows)
    {
        mTextDocs.resize(numRows);
    }
    /**
     * Creates (or retrieves from a cache) the QTextDocument for a given @p index.
     * @param width The width for layouting that QTextDocument. -1 if no layouting is desired (e.g. for converting to text or HTML)
     * @param widget The view to update when fetching thread context on demand. nullptr if this isn't needed (e.g. from SelectionManager)
     * @return the QTextDocument. Ownership remains with the cache, don't delete it.
     */
    QTextDocument *documentForIndex(const QModelIndex &index) const override
    {
        const int row = index.row();
        if (!mTextDocs[row]) {
            const QString text = index.data(MessageModel::MessageConvertedText).toString();
            auto doc = std::make_unique<QTextDocument>();
            doc->setHtml(text);
            mTextDocs[row] = std::move(doc);
        }
        return mTextDocs[row].get();
    }
    QTextDocument *documentForIndex(const MessageAttachment &msgAttach) const override
    {
        // Unused here
        Q_UNUSED(msgAttach);
        Q_ASSERT(true);
        return nullptr;
    }

private:
    mutable std::vector<std::unique_ptr<QTextDocument>> mTextDocs;
};

static QStandardItem *newItem(const QString &text)
{
    auto item = new QStandardItem;
    item->setData(text, MessageModel::MessageConvertedText);
    return item;
}

void TextSelectionTest::initTestCase()
{
    int row = 0;
    model.setItem(row++, 0, newItem(QStringLiteral("Line 0")));
    model.setItem(row++, 0, newItem(QStringLiteral("Line 1 <b>bold</b>")));
    model.setItem(row++, 0, newItem(QStringLiteral("Line 2 <b>bold</b>")));
    model.setItem(row++, 0, newItem(QStringLiteral("Line 3")));
}

void TextSelectionTest::testChangingSelection()
{
    // GIVEN

    const QModelIndex index0 = model.index(0, 0);
    const QModelIndex index1 = model.index(1, 0);
    const QModelIndex index2 = model.index(2, 0);
    const QModelIndex index3 = model.index(3, 0);

    TestFactory factory(model.rowCount());
    TextSelection selection;
    QSignalSpy spy(&selection, &TextSelection::repaintNeeded);

    // WHEN/THEN

    selection.setStart(index1, 3);
    QCOMPARE(selection.selectedText(TextSelection::Text, &factory), QString());
    QVERIFY(!selection.hasSelection());
    selection.setEnd(index1, 4);
    QCOMPARE(selection.selectedText(TextSelection::Text, &factory), QStringLiteral("e"));
    QCOMPARE(spy.count(), 0);
    QVERIFY(selection.hasSelection());

    selection.setEnd(index1, 9);
    QCOMPARE(selection.selectedText(TextSelection::Text, &factory), QStringLiteral("e 1 bo"));
    QCOMPARE(spy.count(), 0);

    spy.clear();
    selection.setEnd(index3, 2);
    QCOMPARE(selection.selectedText(TextSelection::Text, &factory), QStringLiteral("e 1 bold\nLine 2 bold\nLi"));
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(0).at(0).value<QModelIndex>().row(), 1); // line 1 is now fully selected, needs repaint
    QCOMPARE(spy.at(1).at(0).value<QModelIndex>().row(), 2); // line 2 was selected too, needs repaint

    spy.clear();
    selection.setEnd(index2, 2);
    QCOMPARE(selection.selectedText(TextSelection::Text, &factory), QStringLiteral("e 1 bold\nLi"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<QModelIndex>().row(), 3); // line 3 is no longer selected

    auto selectionForRow = [&](int row) {
        const QModelIndex index = model.index(row, 0);
        const QTextCursor cursor = selection.selectionForIndex(index, factory.documentForIndex(index));
        return cursor.selection().toPlainText();
    };
    QCOMPARE(selectionForRow(0), QString());
    QCOMPARE(selectionForRow(1), QStringLiteral("e 1 bold"));
    QCOMPARE(selectionForRow(2), QStringLiteral("Li"));
    QCOMPARE(selectionForRow(3), QString());
    QVERIFY(!selection.contains(index0, 0));
    QVERIFY(!selection.contains(index1, 2));
    QVERIFY(selection.contains(index1, 3));
    QVERIFY(selection.contains(index1, 300));
    QVERIFY(selection.contains(index2, 1));
    QVERIFY(selection.contains(index2, 2)); // (arguable, end of selection)
    QVERIFY(!selection.contains(index2, 3));
    QVERIFY(!selection.contains(index3, 0));

    // Now move up and reverse selection
    spy.clear();
    selection.setEnd(index0, 1);
    QCOMPARE(selection.selectedText(TextSelection::Text, &factory), QStringLiteral("ine 0\nLin"));
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(0).at(0).value<QModelIndex>().row(), 1); // line 1's selection is different
    QCOMPARE(spy.at(1).at(0).value<QModelIndex>().row(), 2); // line 2 is no longer selected
    QCOMPARE(selectionForRow(0), QStringLiteral("ine 0"));
    QCOMPARE(selectionForRow(1), QStringLiteral("Lin"));
    QCOMPARE(selectionForRow(2), QString());
    QCOMPARE(selectionForRow(3), QString());
    QVERIFY(!selection.contains(index0, 0));
    QVERIFY(selection.contains(index0, 1));
    QVERIFY(selection.contains(index0, 100));
    QVERIFY(selection.contains(index1, 0));
    QVERIFY(selection.contains(index1, 2));
    QVERIFY(selection.contains(index1, 3)); // (arguable, end of selection)
    QVERIFY(!selection.contains(index2, 0));
    QVERIFY(!selection.contains(index3, 0));
}

void TextSelectionTest::testSingleLineReverseSelection()
{
    // GIVEN
    const QModelIndex index1 = model.index(1, 0);
    TestFactory factory(model.rowCount());
    TextSelection selection;

    // WHEN
    selection.setStart(index1, 4);
    selection.setEnd(index1, 1);

    // THEN
    QCOMPARE(selection.selectedText(TextSelection::Text, &factory), QStringLiteral("ine"));
}

void TextSelectionTest::testSelectWordUnderCursor()
{
    // GIVEN
    const QModelIndex index1 = model.index(1, 0);
    const QModelIndex index2 = model.index(2, 0);
    TestFactory factory(model.rowCount());
    TextSelection selection;

    // WHEN
    selection.selectWordUnderCursor(index1, 2, &factory);

    // THEN
    QCOMPARE(selection.selectedText(TextSelection::Text, &factory), QStringLiteral("Line"));
    QVERIFY(selection.contains(index1, 0));
    QVERIFY(selection.contains(index1, 2));
    QVERIFY(selection.contains(index1, 4));
    QVERIFY(!selection.contains(index1, 5));

    // and again on another row

    // WHEN
    selection.setStart(index2, 8);
    selection.setEnd(index2, 8); // tiny mouse move while double-clicking
    QVERIFY(!selection.hasSelection());
    selection.selectWordUnderCursor(index2, 8, &factory);

    // THEN
    QCOMPARE(selection.selectedText(TextSelection::Text, &factory), QStringLiteral("bold"));
    QVERIFY(!selection.contains(index2, 0));
    QVERIFY(!selection.contains(index2, 6));
    QVERIFY(selection.contains(index2, 7));
    QVERIFY(selection.contains(index2, 9));
}
