/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textselectiontest.h"

#include "delegateutils/textselection.h"
#include "messages/messageurl.h"
#include "model/messagesmodel.h"

#include <QSignalSpy>
#include <QStandardPaths>
#include <QTest>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextDocumentFragment>

QTEST_MAIN(TextSelectionTest)

using namespace Qt::Literals::StringLiterals;
TextSelectionTest::TextSelectionTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
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
            const QString text = index.data(MessagesModel::MessageConvertedText).toString();
            auto doc = std::make_unique<QTextDocument>();
            doc->setHtml(text);
            mTextDocs[row] = std::move(doc);
        }
        return mTextDocs[row].get();
    }

private:
    mutable std::vector<std::unique_ptr<QTextDocument>> mTextDocs;
};

class TestUrlPreviewFactory : public DocumentFactoryInterface
{
public:
    QTextDocument *documentForUrlPreview(const MessageUrl &messageUrl) const override
    {
        if (!mDoc) {
            mDoc = std::make_unique<QTextDocument>();
            mDoc->setHtml(messageUrl.htmlDescription());
        }
        return mDoc.get();
    }

private:
    mutable std::unique_ptr<QTextDocument> mDoc;
};

static QStandardItem *newItem(const QString &text)
{
    auto item = new QStandardItem;
    item->setData(text, MessagesModel::MessageConvertedText);
    return item;
}

void TextSelectionTest::initTestCase()
{
    int row = 0;
    model.setItem(row++, 0, newItem(u"Line 0"_s));
    model.setItem(row++, 0, newItem(u"Line 1 <b>bold</b>"_s));
    model.setItem(row++, 0, newItem(u"Line 2 <b>bold</b>"_s));
    model.setItem(row++, 0, newItem(u"Line 3"_s));
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
    selection.setTextHelperFactory(&factory);
    QSignalSpy spy(&selection, &TextSelection::repaintNeeded);

    // WHEN/THEN

    selection.setTextSelectionStart(index1, 3);
    QCOMPARE(selection.selectedText(TextSelection::Format::Text), QString());
    QVERIFY(!selection.hasSelection());
    selection.setTextSelectionEnd(index1, 4);
    QCOMPARE(selection.selectedText(TextSelection::Format::Text), u"e"_s);
    QCOMPARE(spy.count(), 0);
    QVERIFY(selection.hasSelection());

    selection.setTextSelectionEnd(index1, 9);
    QCOMPARE(selection.selectedText(TextSelection::Format::Text), u"e 1 bo"_s);
    QCOMPARE(spy.count(), 0);

    spy.clear();
    selection.setTextSelectionEnd(index3, 2);
    QCOMPARE(selection.selectedText(TextSelection::Format::Text), u"e 1 bold\nLine 2 bold\nLi"_s);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(0).at(0).value<QModelIndex>().row(), 1); // line 1 is now fully selected, needs repaint
    QCOMPARE(spy.at(1).at(0).value<QModelIndex>().row(), 2); // line 2 was selected too, needs repaint

    spy.clear();
    selection.setTextSelectionEnd(index2, 2);
    QCOMPARE(selection.selectedText(TextSelection::Format::Text), u"e 1 bold\nLi"_s);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<QModelIndex>().row(), 3); // line 3 is no longer selected

    auto selectionForRow = [&](int row) {
        const QModelIndex index = model.index(row, 0);
        const QTextCursor cursor = selection.selectionForIndex(index, factory.documentForIndex(index));
        return cursor.selection().toPlainText();
    };
    QCOMPARE(selectionForRow(0), QString());
    QCOMPARE(selectionForRow(1), u"e 1 bold"_s);
    QCOMPARE(selectionForRow(2), u"Li"_s);
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
    selection.setTextSelectionEnd(index0, 1);
    QCOMPARE(selection.selectedText(TextSelection::Format::Text), u"ine 0\nLin"_s);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(0).at(0).value<QModelIndex>().row(), 1); // line 1's selection is different
    QCOMPARE(spy.at(1).at(0).value<QModelIndex>().row(), 2); // line 2 is no longer selected
    QCOMPARE(selectionForRow(0), u"ine 0"_s);
    QCOMPARE(selectionForRow(1), u"Lin"_s);
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
    selection.setTextHelperFactory(&factory);

    // WHEN
    selection.setTextSelectionStart(index1, 4);
    selection.setTextSelectionEnd(index1, 1);

    // THEN
    QCOMPARE(selection.selectedText(TextSelection::Format::Text), u"ine"_s);
    // ... and the selected range must be reported as selected, just like for a left-to-right selection.
    QVERIFY(!selection.contains(index1, 0));
    QVERIFY(selection.contains(index1, 1));
    QVERIFY(selection.contains(index1, 2));
    QVERIFY(selection.contains(index1, 4)); // (arguable, end of selection)
    QVERIFY(!selection.contains(index1, 5));
}

void TextSelectionTest::testSelectWordUnderCursor()
{
    // GIVEN
    const QModelIndex index1 = model.index(1, 0);
    const QModelIndex index2 = model.index(2, 0);
    TestFactory factory(model.rowCount());
    TextSelection selection;
    selection.setTextHelperFactory(&factory);

    // WHEN
    selection.selectWordUnderCursor(index1, 2, &factory);

    // THEN
    QCOMPARE(selection.selectedText(TextSelection::Format::Text), u"Line"_s);
    QVERIFY(selection.contains(index1, 0));
    QVERIFY(selection.contains(index1, 2));
    QVERIFY(selection.contains(index1, 4));
    QVERIFY(!selection.contains(index1, 5));

    // and again on another row

    // WHEN
    selection.setTextSelectionStart(index2, 8);
    selection.setTextSelectionEnd(index2, 8); // tiny mouse move while double-clicking
    QVERIFY(!selection.hasSelection());
    selection.selectWordUnderCursor(index2, 8, &factory);

    // THEN
    QCOMPARE(selection.selectedText(TextSelection::Format::Text), u"bold"_s);
    QVERIFY(!selection.contains(index2, 0));
    QVERIFY(!selection.contains(index2, 6));
    QVERIFY(selection.contains(index2, 7));
    QVERIFY(selection.contains(index2, 9));
}

void TextSelectionTest::testSelectWordUnderCursorInUrlPreviewDoesNotSelectMessageText()
{
    // GIVEN
    const QModelIndex index1 = model.index(1, 0);
    TestFactory factory(model.rowCount());
    TestUrlPreviewFactory urlPreviewFactory;
    TextSelection selection;
    selection.setTextHelperFactory(&factory);
    selection.setMessageUrlHelperFactory(&urlPreviewFactory);

    MessageUrl messageUrl;
    messageUrl.setUrl(u"https://kde.org"_s);
    messageUrl.setPageTitle(u"KDE"_s);
    messageUrl.setDescription(u"Community"_s);
    messageUrl.generateMessageUrlInfo();
    QVERIFY(messageUrl.hasHtmlDescription());

    QTextDocument *urlPreviewDoc = urlPreviewFactory.documentForUrlPreview(messageUrl);
    const QTextCursor found = urlPreviewDoc->find(u"KDE"_s);
    QVERIFY(!found.isNull());
    // Use a word whose position also exists in the message text, so that a leaking position is visible.
    QVERIFY(found.selectionEnd() < factory.documentForIndex(index1)->characterCount() - 1);

    // WHEN double-clicking a word inside the url preview
    selection.selectWordUnderCursor(index1, found.selectionStart() + 1, &urlPreviewFactory, messageUrl);

    // THEN the word is selected in the url preview...
    QVERIFY(selection.hasSelection());
    const QTextCursor urlCursor = selection.selectionForIndex(index1, urlPreviewDoc, {}, messageUrl);
    QVERIFY(!urlCursor.isNull());
    QCOMPARE(urlCursor.selection().toPlainText(), u"KDE"_s);

    // ... and the message text of that row stays untouched.
    const QTextCursor messageCursor = selection.selectionForIndex(index1, factory.documentForIndex(index1));
    QVERIFY(messageCursor.isNull() || messageCursor.selection().toPlainText().isEmpty());
    // The test model has no Message, so the url preview text can't be collected here; what matters
    // is that no part of "Line 1 bold" ends up in the selected text.
    QVERIFY(selection.selectedText(TextSelection::Format::Text).isEmpty());
}

void TextSelectionTest::shouldHaveDefaultValues()
{
    const TextSelection selection;
    QVERIFY(!selection.hasSelection());
    QVERIFY(!selection.textHelperFactory());
    QVERIFY(selection.attachmentFactories().isEmpty());
}

void TextSelectionTest::testSelectAll()
{
    // GIVEN
    const QModelIndex index1 = model.index(1, 0);
    TestFactory factory(model.rowCount());
    TextSelection selection;
    selection.setTextHelperFactory(&factory);

    // WHEN
    QVERIFY(!selection.hasSelection());
    selection.selectMessage(index1);
    QVERIFY(selection.hasSelection());
    QCOMPARE(selection.selectedText(TextSelection::Format::Text), u"Line 1 bold"_s);
}

void TextSelectionTest::testSelectionForIndexDoesNotIncludeUrlPreviewByDefault()
{
    const QModelIndex index1 = model.index(1, 0);
    TestFactory factory(model.rowCount());
    TextSelection selection;
    selection.setTextHelperFactory(&factory);

    selection.setTextSelectionStart(index1, 0);
    selection.setTextSelectionEnd(index1, 4);

    MessageUrl messageUrl;
    messageUrl.setUrl(u"https://kde.org"_s);
    messageUrl.setPageTitle(u"KDE"_s);
    messageUrl.setDescription(u"Community"_s);
    messageUrl.generateMessageUrlInfo();
    QVERIFY(messageUrl.hasHtmlDescription());

    QTextDocument urlPreviewDoc;
    urlPreviewDoc.setHtml(messageUrl.htmlDescription());

    const QTextCursor cursor = selection.selectionForIndex(index1, &urlPreviewDoc, {}, messageUrl);
    QVERIFY(cursor.isNull());
}

void TextSelectionTest::testSelectionExtendingToUrlPreviewKeepsTextSelection()
{
    const QModelIndex index1 = model.index(1, 0);
    TestFactory factory(model.rowCount());
    TextSelection selection;
    selection.setTextHelperFactory(&factory);

    selection.setTextSelectionStart(index1, 0);
    selection.setTextSelectionEnd(index1, 4);

    MessageUrl messageUrl;
    messageUrl.setUrl(u"https://kde.org"_s);
    messageUrl.setPageTitle(u"KDE"_s);
    messageUrl.setDescription(u"Community"_s);
    messageUrl.generateMessageUrlInfo();
    QVERIFY(messageUrl.hasHtmlDescription());

    selection.setPreviewUrlTextSelectionEnd(index1, 366, messageUrl);

    QTextDocument urlPreviewDoc;
    urlPreviewDoc.setHtml(messageUrl.htmlDescription());

    const QTextCursor messageCursor = selection.selectionForIndex(index1, factory.documentForIndex(index1));
    QVERIFY(!messageCursor.isNull());
    QCOMPARE(messageCursor.selection().toPlainText(), u"Line"_s);

    const QTextCursor urlCursor = selection.selectionForIndex(index1, &urlPreviewDoc, {}, messageUrl);
    QVERIFY(!urlCursor.isNull());
    QCOMPARE(urlCursor.position(), urlCursor.anchor());
}

void TextSelectionTest::testSelectionFromTextToUrlDoesNotSelectUrlRowTextUntilTextIsHit()
{
    const QModelIndex index1 = model.index(1, 0);
    const QModelIndex index2 = model.index(2, 0);
    TestFactory factory(model.rowCount());
    TextSelection selection;
    selection.setTextHelperFactory(&factory);

    MessageUrl messageUrl;
    messageUrl.setUrl(u"https://kde.org"_s);
    messageUrl.setPageTitle(u"KDE"_s);
    messageUrl.setDescription(u"Community"_s);
    messageUrl.generateMessageUrlInfo();
    QVERIFY(messageUrl.hasHtmlDescription());

    // Start from message text in a lower row.
    selection.setTextSelectionStart(index2, 0);
    selection.setTextSelectionEnd(index2, 11);

    // Move endpoint up into URL preview (same row as index1 message).
    selection.setPreviewUrlTextSelectionEnd(index1, 2, messageUrl);
    selection.setPreviewUrlTextSelectionEnd(index1, 12, messageUrl);

    const QTextCursor row2Cursor = selection.selectionForIndex(index2, factory.documentForIndex(index2));
    QVERIFY(!row2Cursor.isNull());
    QCOMPARE(row2Cursor.selection().toPlainText(), u"Line 2 bold"_s);

    // While endpoint is in URL preview on row 1, row 1 message text must stay unselected.
    const QTextCursor row1Cursor = selection.selectionForIndex(index1, factory.documentForIndex(index1));
    QVERIFY(row1Cursor.isNull() || row1Cursor.selection().toPlainText().isEmpty());

    QTextDocument urlPreviewDoc;
    urlPreviewDoc.setHtml(messageUrl.htmlDescription());
    const QTextCursor urlCursor = selection.selectionForIndex(index1, &urlPreviewDoc, {}, messageUrl);
    QVERIFY(!urlCursor.isNull());
    QVERIFY(!urlCursor.selection().toPlainText().isEmpty());
}

void TextSelectionTest::testSelectionStartingInUrlPreviewAndMovingToText()
{
    const QModelIndex index1 = model.index(1, 0);
    TestFactory factory(model.rowCount());
    TextSelection selection;
    selection.setTextHelperFactory(&factory);

    MessageUrl messageUrl;
    messageUrl.setUrl(u"https://kde.org"_s);
    messageUrl.setPageTitle(u"KDE"_s);
    messageUrl.setDescription(u"Community"_s);
    messageUrl.generateMessageUrlInfo();
    QVERIFY(messageUrl.hasHtmlDescription());

    QTextDocument urlPreviewDoc;
    urlPreviewDoc.setHtml(messageUrl.htmlDescription());

    selection.setPreviewUrlTextSelectionStart(index1, 366, messageUrl);

    // Move the selection endpoint to the main message text (drag up).
    selection.setTextSelectionEnd(index1, 4);

    const QTextCursor messageCursor = selection.selectionForIndex(index1, factory.documentForIndex(index1));
    QVERIFY(!messageCursor.isNull());
    QCOMPARE(messageCursor.selection().toPlainText(), u" 1 bold"_s);

    const QTextCursor urlCursor = selection.selectionForIndex(index1, &urlPreviewDoc, {}, messageUrl);
    QVERIFY(!urlCursor.isNull());
    // Starting in URL preview and immediately dragging to message text can keep a zero-length URL cursor.
    QCOMPARE(urlCursor.position(), urlCursor.anchor());
}

void TextSelectionTest::testSelectionStartingInUrlPreviewAndMovingToPreviousMessage()
{
    const QModelIndex index0 = model.index(0, 0);
    const QModelIndex index2 = model.index(2, 0);
    TestFactory factory(model.rowCount());
    TextSelection selection;
    selection.setTextHelperFactory(&factory);

    MessageUrl messageUrl;
    messageUrl.setUrl(u"https://kde.org"_s);
    messageUrl.setPageTitle(u"KDE"_s);
    messageUrl.setDescription(u"Community"_s);
    messageUrl.generateMessageUrlInfo();
    QVERIFY(messageUrl.hasHtmlDescription());

    selection.setPreviewUrlTextSelectionStart(index2, 2, messageUrl);
    selection.setPreviewUrlTextSelectionEnd(index2, 12, messageUrl);

    // Move the selection endpoint two rows up.
    selection.setTextSelectionEnd(index0, 4);

    const QTextCursor row0Cursor = selection.selectionForIndex(index0, factory.documentForIndex(index0));
    QVERIFY(!row0Cursor.isNull());
    QCOMPARE(row0Cursor.selection().toPlainText(), u" 0"_s);

    const QTextCursor row2Cursor = selection.selectionForIndex(index2, factory.documentForIndex(index2));
    QVERIFY(!row2Cursor.isNull());
    QCOMPARE(row2Cursor.selection().toPlainText(), u"Line 2 bold"_s);

    QTextDocument urlPreviewDoc;
    urlPreviewDoc.setHtml(messageUrl.htmlDescription());
    const QTextCursor urlCursor = selection.selectionForIndex(index2, &urlPreviewDoc, {}, messageUrl);
    QVERIFY(!urlCursor.isNull());
    QVERIFY(!urlCursor.selection().toPlainText().isEmpty());
}

void TextSelectionTest::textClear()
{
    // GIVEN
    const QModelIndex index1 = model.index(1, 0);
    TestFactory factory(model.rowCount());
    TextSelection selection;
    selection.setTextHelperFactory(&factory);

    // WHEN
    QVERIFY(!selection.hasSelection());
    selection.selectMessage(index1);
    QVERIFY(selection.hasSelection());
    selection.clear();
    QVERIFY(!selection.hasSelection());
}

#include "moc_textselectiontest.cpp"
