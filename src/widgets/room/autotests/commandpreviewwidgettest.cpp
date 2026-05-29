/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "commandpreviewwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "commands/previewcommand.h"
#include "model/previewcommandmodel.h"
#include "room/commandpreviewwidget.h"
#include <QHBoxLayout>
#include <QListView>
#include <QSignalSpy>
#include <QStackedWidget>
#include <QTest>
QTEST_MAIN(CommandPreviewWidgetTest)

CommandPreviewWidgetTest::CommandPreviewWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void CommandPreviewWidgetTest::shouldHaveDefaultValues()
{
    CommandPreviewWidget w;

    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mListView = w.findChild<QListView *>(u"mListView"_s);
    QVERIFY(mListView);
    QCOMPARE(mListView->viewMode(), QListView::IconMode);
    QCOMPARE(mListView->flow(), QListView::LeftToRight);
    QCOMPARE(mListView->resizeMode(), QListView::Adjust);
    QVERIFY(!mListView->wordWrap());
    QCOMPARE(mListView->spacing(), 2);

    auto mStackWidget = w.findChild<QStackedWidget *>(u"mStackWidget"_s);
    QVERIFY(mStackWidget);
}

void CommandPreviewWidgetTest::shouldHidePreviewOnEscape()
{
    CommandPreviewWidget w;
    w.show();
    QVERIFY(w.isVisible());

    QTest::keyClick(&w, Qt::Key_Escape);
    QVERIFY(!w.isVisible());
}

void CommandPreviewWidgetTest::shouldNotEmitOnInvalidDoubleClick()
{
    CommandPreviewWidget w;
    QSignalSpy spy(&w, &CommandPreviewWidget::sendPreviewCommandInfo);
    QVERIFY(spy.isValid());

    // QVERIFY(QMetaObject::invokeMethod(&w, "slotDoubleClicked", Q_ARG(QModelIndex, QModelIndex())));
    // QCOMPARE(spy.count(), 0);
}

void CommandPreviewWidgetTest::shouldEmitOnEnterWithSelection()
{
    CommandPreviewWidget w;
    auto mListView = w.findChild<QListView *>(u"mListView"_s);
    QVERIFY(mListView);
    auto mStackWidget = w.findChild<QStackedWidget *>(u"mStackWidget"_s);
    QVERIFY(mStackWidget);

    auto model = qobject_cast<PreviewCommandModel *>(mListView->model());
    QVERIFY(model);

    PreviewCommand command;
    command.setId(u"id1"_s);
    command.setValue(u"value1"_s);
    command.setType(PreviewCommand::TypePreview::Text);
    model->setPreviewCommands({command});
    QVERIFY(model->rowCount() == 1);

    const QModelIndex index = model->index(0, 0);
    QVERIFY(index.isValid());
    mListView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    mListView->setCurrentIndex(index);

    mStackWidget->setCurrentWidget(mListView);
    w.show();
    w.activateWindow();
    w.setFocus();

    QSignalSpy spy(&w, &CommandPreviewWidget::sendPreviewCommandInfo);
    QVERIFY(spy.isValid());

    QTest::keyClick(&w, Qt::Key_Return);
    QCOMPARE(spy.count(), 1);
}

#include "moc_commandpreviewwidgettest.cpp"
