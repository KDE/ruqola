/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportuserdialogtest.h"
#include "dialogs/reportuserdialog.h"
#include "dialogs/reportuserwidget.h"
#include <KTextEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ReportUserDialogTest)

ReportUserDialogTest::ReportUserDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ReportUserDialogTest::shouldHaveDefaultValues()
{
    ReportUserDialog w;
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mReportUserWidget = w.findChild<ReportUserWidget *>(QStringLiteral("mReportUserWidget"));
    QVERIFY(mReportUserWidget);

    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    QVERIFY(w.message().isEmpty());

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    QVERIFY(!okButton->isEnabled());
}

void ReportUserDialogTest::shouldUpdateOkButton()
{
    ReportUserDialog w;
    auto mReportUserWidget = w.findChild<ReportUserWidget *>(QStringLiteral("mReportUserWidget"));
    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));

    auto mMessageLineEdit = mReportUserWidget->findChild<KTextEdit *>(QStringLiteral("mMessageLineEdit"));
    QVERIFY(mMessageLineEdit);
    mMessageLineEdit->setText(QStringLiteral("foo"));
    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    QVERIFY(okButton->isEnabled());
}

#include "moc_reportuserdialogtest.cpp"
