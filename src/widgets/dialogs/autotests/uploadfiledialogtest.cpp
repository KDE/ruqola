/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfiledialogtest.h"
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
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mUploadFileWidget = w.findChild<UploadFileWidget *>(QStringLiteral("mUploadFileWidget"));
    QVERIFY(mUploadFileWidget);
    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("buttonBox"));
    QVERIFY(buttonBox);

    QPushButton *mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    QVERIFY(!mOkButton->isEnabled());
}

#include "moc_uploadfiledialogtest.cpp"
