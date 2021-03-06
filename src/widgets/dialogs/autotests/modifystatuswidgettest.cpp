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

#include "modifystatuswidgettest.h"
#include "dialogs/modifystatuswidget.h"
#include "misc/statuscombobox.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(ModifyStatusWidgetTest)

ModifyStatusWidgetTest::ModifyStatusWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ModifyStatusWidgetTest::shouldHaveDefaultValues()
{
    ModifyStatusWidget w;
    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto mStatusCombobox = w.findChild<StatusCombobox *>(QStringLiteral("mStatusCombobox"));
    QVERIFY(mStatusCombobox);

    auto mStatusLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mStatusLineEdit"));
    QVERIFY(mStatusLineEdit);
    QVERIFY(mStatusLineEdit->isClearButtonEnabled());

    QVERIFY(w.messageStatus().isEmpty());
}

void ModifyStatusWidgetTest::changeStatusMessage()
{
    ModifyStatusWidget w;
    const QString messages = QStringLiteral("messages");
    w.setMessageStatus(messages);
    QCOMPARE(w.messageStatus(), messages);
}
