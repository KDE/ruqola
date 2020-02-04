/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "searchmessagewidgettest.h"
#include "dialogs/searchmessagewidget.h"
#include "room/messagelistview.h"
#include <KLineEdit>
#include <QTest>
QTEST_MAIN(SearchMessageWidgetTest)

SearchMessageWidgetTest::SearchMessageWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void SearchMessageWidgetTest::shouldHaveDefaultValues()
{
    SearchMessageWidget w;

    KLineEdit *mSearchLineEdit = w.findChild<KLineEdit *>(QStringLiteral("mSearchLineEdit"));
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());
    QVERIFY(mSearchLineEdit->trapReturnKey());
    QVERIFY(!mSearchLineEdit->placeholderText().isEmpty());

    MessageListView *mResultListWidget = w.findChild<MessageListView *>(QStringLiteral("mResultListWidget"));
    QVERIFY(mResultListWidget);
    QCOMPARE(mResultListWidget->mode(), MessageListView::Mode::Viewing);
}
