/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
    ShowAttachmentWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto searchAttachmentLayout = w.findChild<QHBoxLayout *>(QStringLiteral("searchAttachmentLayout"));
    QVERIFY(searchAttachmentLayout);
    QCOMPARE(searchAttachmentLayout->contentsMargins(), {});

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
