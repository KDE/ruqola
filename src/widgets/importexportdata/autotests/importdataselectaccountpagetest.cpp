/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importdataselectaccountpagetest.h"
using namespace Qt::Literals::StringLiterals;

#include "importexportdata/importdata/importdataselectaccountpage.h"
#include <KUrlRequester>
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ImportDataSelectAccountPageTest)
ImportDataSelectAccountPageTest::ImportDataSelectAccountPageTest(QObject *parent)
    : QObject{parent}
{
}

void ImportDataSelectAccountPageTest::shouldHaveDefaultValues()
{
    ImportDataSelectAccountPage w;

    auto mUrlRequester = w.findChild<KUrlRequester *>(u"mUrlRequester"_s);
    QVERIFY(mUrlRequester);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    QVERIFY(w.zipFileUrl().isEmpty());
}

#include "moc_importdataselectaccountpagetest.cpp"
