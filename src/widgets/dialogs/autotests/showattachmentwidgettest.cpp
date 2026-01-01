/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showattachmentwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/showattachmentcombobox.h"
#include "dialogs/showattachmentwidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ShowAttachmentWidgetTest)
ShowAttachmentWidgetTest::ShowAttachmentWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ShowAttachmentWidgetTest::shouldHaveDefaultValues()
{
    ShowAttachmentWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto searchAttachmentLayout = w.findChild<QHBoxLayout *>(u"searchAttachmentLayout"_s);
    QVERIFY(searchAttachmentLayout);
    QCOMPARE(searchAttachmentLayout->contentsMargins(), QMargins{});

    auto mSearchAttachmentFileLineEdit = w.findChild<QLineEdit *>(u"mSearchAttachmentFileLineEdit"_s);
    QVERIFY(mSearchAttachmentFileLineEdit);
    QVERIFY(mSearchAttachmentFileLineEdit->isClearButtonEnabled());
    QVERIFY(!mSearchAttachmentFileLineEdit->placeholderText().isEmpty());

    auto mAttachmentCombobox = w.findChild<ShowAttachmentComboBox *>(u"mAttachmentCombobox"_s);
    QVERIFY(mAttachmentCombobox);

    auto mInfo = w.findChild<QLabel *>(u"mInfo"_s);
    QVERIFY(mInfo);
    QVERIFY(mInfo->text().isEmpty());
    QCOMPARE(mInfo->contextMenuPolicy(), Qt::NoContextMenu);

    auto mListAttachment = w.findChild<QListView *>(u"mListAttachment"_s);
    QVERIFY(mListAttachment);
}

#include "moc_showattachmentwidgettest.cpp"
