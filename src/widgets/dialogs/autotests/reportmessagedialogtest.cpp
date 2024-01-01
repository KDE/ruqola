/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportmessagedialogtest.h"
#include "dialogs/reportmessagedialog.h"
#include "dialogs/reportmessagewidget.h"
#include <KTextEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ReportMessageDialogTest)

ReportMessageDialogTest::ReportMessageDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ReportMessageDialogTest::shouldHaveDefaultValues()
{
    ReportMessageDialog w;
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mReportMessageWidget = w.findChild<ReportMessageWidget *>(QStringLiteral("mReportMessageWidget"));
    QVERIFY(mReportMessageWidget);

    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    QVERIFY(w.message().isEmpty());

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    QVERIFY(!okButton->isEnabled());
}

void ReportMessageDialogTest::shouldUpdateOkButton()
{
    ReportMessageDialog w;
    auto mReportMessageWidget = w.findChild<ReportMessageWidget *>(QStringLiteral("mReportMessageWidget"));
    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));

    auto mMessageLineEdit = mReportMessageWidget->findChild<KTextEdit *>(QStringLiteral("mMessageLineEdit"));
    QVERIFY(mMessageLineEdit);
    mMessageLineEdit->setText(QStringLiteral("foo"));
    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    QVERIFY(okButton->isEnabled());
}

#include "moc_reportmessagedialogtest.cpp"
