/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfiledialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/uploadfiledialog.h"
#include "dialogs/uploadfilewidget.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(UploadFileDialogTest)

UploadFileDialogTest::UploadFileDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void UploadFileDialogTest::shouldHaveDefaultValues()
{
    UploadFileDialog w;
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mUploadFileWidget = w.findChild<UploadFileWidget *>(u"mUploadFileWidget"_s);
    QVERIFY(mUploadFileWidget);
    auto buttonBox = w.findChild<QDialogButtonBox *>(u"buttonBox"_s);
    QVERIFY(buttonBox);
}

#include "moc_uploadfiledialogtest.cpp"
