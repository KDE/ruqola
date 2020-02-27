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

#include "reportmessagedialogtest.h"
#include "dialogs/reportmessagedialog.h"
#include "dialogs/reportmessagewidget.h"
#include <QTest>
#include <QStandardPaths>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLineEdit>
#include <KTextEdit>

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

    auto *mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto *mReportMessageWidget = w.findChild<ReportMessageWidget *>(QStringLiteral("mReportMessageWidget"));
    QVERIFY(mReportMessageWidget);

    auto *button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    QVERIFY(w.message().isEmpty());

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    QVERIFY(!okButton->isEnabled());
}

void ReportMessageDialogTest::shouldUpdateOkButton()
{
    ReportMessageDialog w;
    auto *mReportMessageWidget = w.findChild<ReportMessageWidget *>(QStringLiteral("mReportMessageWidget"));
    auto *button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));

    auto *mMessageLineEdit = mReportMessageWidget->findChild<KTextEdit *>(QStringLiteral("mMessageLineEdit"));
    QVERIFY(mMessageLineEdit);
    mMessageLineEdit->setText(QStringLiteral("foo"));
    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    QVERIFY(okButton->isEnabled());
}
