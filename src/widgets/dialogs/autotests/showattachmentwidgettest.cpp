/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showattachmentwidgettest.h"
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
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto searchAttachmentLayout = w.findChild<QHBoxLayout *>(QStringLiteral("searchAttachmentLayout"));
    QVERIFY(searchAttachmentLayout);
    QCOMPARE(searchAttachmentLayout->contentsMargins(), QMargins{});

    auto mSearchAttachmentFileLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mSearchAttachmentFileLineEdit"));
    QVERIFY(mSearchAttachmentFileLineEdit);
    QVERIFY(mSearchAttachmentFileLineEdit->isClearButtonEnabled());
    QVERIFY(!mSearchAttachmentFileLineEdit->placeholderText().isEmpty());

    auto mAttachmentCombobox = w.findChild<ShowAttachmentComboBox *>(QStringLiteral("mAttachmentCombobox"));
    QVERIFY(mAttachmentCombobox);

    auto mInfo = w.findChild<QLabel *>(QStringLiteral("mInfo"));
    QVERIFY(mInfo);
    QVERIFY(mInfo->text().isEmpty());
    QCOMPARE(mInfo->contextMenuPolicy(), Qt::NoContextMenu);

    auto mListAttachment = w.findChild<QListView *>(QStringLiteral("mListAttachment"));
    QVERIFY(mListAttachment);
}

#include "moc_showattachmentwidgettest.cpp"
