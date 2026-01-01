/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ecopypassworddialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "encryption/e2ecopypassworddialog.h"
#include "encryption/e2ecopypasswordwidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(E2eCopyPasswordDialogTest)
E2eCopyPasswordDialogTest::E2eCopyPasswordDialogTest(QObject *parent)
    : QObject{parent}
{
}

void E2eCopyPasswordDialogTest::shouldHaveDefaultValues()
{
    E2eCopyPasswordDialog d(nullptr);

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto buttonBox = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(buttonBox);
    QCOMPARE(buttonBox->standardButtons(), QDialogButtonBox::StandardButtons{QDialogButtonBox::Cancel | QDialogButtonBox::Ok});

    auto mE2eCopyPasswordWidget = d.findChild<E2eCopyPasswordWidget *>(u"mE2eCopyPasswordWidget"_s);
    QVERIFY(mE2eCopyPasswordWidget);
}

#include "moc_e2ecopypassworddialogtest.cpp"
