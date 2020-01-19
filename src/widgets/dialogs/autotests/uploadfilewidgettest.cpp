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

#include "uploadfilewidgettest.h"
#include "dialogs/uploadfilewidget.h"
#include <KUrlRequester>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(UploadFileWidgetTest)

UploadFileWidgetTest::UploadFileWidgetTest(QObject *parent)
    : QObject(parent)
{

}

void UploadFileWidgetTest::shouldHaveDefaultValues()
{
    UploadFileWidget w;
    QFormLayout *layout = w.findChild<QFormLayout *>(QStringLiteral("layout"));
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins(0, 0, 0, 0));

    QLineEdit *mDescription = w.findChild<QLineEdit *>(QStringLiteral("mDescription"));
    QVERIFY(mDescription);
    QVERIFY(mDescription->isClearButtonEnabled());

    QLineEdit *mMessage = w.findChild<QLineEdit *>(QStringLiteral("mMessage"));
    QVERIFY(mMessage);
    QVERIFY(mMessage->isClearButtonEnabled());

    KUrlRequester *mSelectFile = w.findChild<KUrlRequester *>(QStringLiteral("mSelectFile"));
    QVERIFY(mSelectFile);
}

void UploadFileWidgetTest::shouldReturnEmptyResult()
{
    UploadFileWidget w;
    QVERIFY(w.description().isEmpty());
    QVERIFY(w.message().isEmpty());
    QVERIFY(w.fileUrl().isEmpty());
}
