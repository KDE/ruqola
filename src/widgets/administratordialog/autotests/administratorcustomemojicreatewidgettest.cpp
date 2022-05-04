/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomemojicreatewidgettest.h"
#include "administratordialog/customemoji/administratorcustomemojicreatewidget.h"

#include <KUrlRequester>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(AdministratorCustomEmojiCreateWidgetTest)
AdministratorCustomEmojiCreateWidgetTest::AdministratorCustomEmojiCreateWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorCustomEmojiCreateWidgetTest::shouldHaveDefaultValues()
{
    AdministratorCustomEmojiCreateWidget w;
    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mName = w.findChild<QLineEdit *>(QStringLiteral("mName"));
    QVERIFY(mName);
    QVERIFY(mName->text().isEmpty());
    QVERIFY(mName->isClearButtonEnabled());

    auto mAlias = w.findChild<QLineEdit *>(QStringLiteral("mAlias"));
    QVERIFY(mAlias);
    QVERIFY(mAlias->text().isEmpty());
    QVERIFY(mAlias->isClearButtonEnabled());

    auto mSelectFile = w.findChild<KUrlRequester *>(QStringLiteral("mSelectFile"));
    QVERIFY(mSelectFile);

    auto mWarningLabel = w.findChild<QLabel *>(QStringLiteral("mWarningLabel"));
    QVERIFY(mWarningLabel);
    QVERIFY(mWarningLabel->isHidden());
}

void AdministratorCustomEmojiCreateWidgetTest::shouldEmitSignal()
{
    AdministratorCustomEmojiCreateWidget w;
    QSignalSpy updateOkButtonChanged(&w, &AdministratorCustomEmojiCreateWidget::updateOkButton);

    auto mName = w.findChild<QLineEdit *>(QStringLiteral("mName"));
    auto mAlias = w.findChild<QLineEdit *>(QStringLiteral("mAlias"));
    auto mSelectFile = w.findChild<KUrlRequester *>(QStringLiteral("mSelectFile"));
    auto mWarningLabel = w.findChild<QLabel *>(QStringLiteral("mWarningLabel"));

    const QString name = QStringLiteral("bla");
    mName->setText(name);
    QCOMPARE(updateOkButtonChanged.count(), 1);
    QVERIFY(!updateOkButtonChanged.at(0).at(0).toBool());
    updateOkButtonChanged.clear();
    QVERIFY(mWarningLabel->isHidden());

    mAlias->setText(QStringLiteral("bli"));
    QCOMPARE(updateOkButtonChanged.count(), 1);
    QVERIFY(!updateOkButtonChanged.at(0).at(0).toBool());
    updateOkButtonChanged.clear();
    QVERIFY(mWarningLabel->isHidden());

    // Valid
    mSelectFile->setUrl(QUrl::fromLocalFile(QStringLiteral("/tmp/bla")));
    QCOMPARE(updateOkButtonChanged.count(), 1);
    QVERIFY(updateOkButtonChanged.at(0).at(0).toBool());
    updateOkButtonChanged.clear();

    mAlias->clear();
    QCOMPARE(updateOkButtonChanged.count(), 1);
    // Always valid when we clear alias
    QVERIFY(updateOkButtonChanged.at(0).at(0).toBool());
    updateOkButtonChanged.clear();

    // Valid
    mSelectFile->setUrl(QUrl::fromLocalFile(QStringLiteral("/tmp/bla2")));
    QCOMPARE(updateOkButtonChanged.count(), 1);
    QVERIFY(updateOkButtonChanged.at(0).at(0).toBool());
    updateOkButtonChanged.clear();

    // same name and alias => disable ok button
    mAlias->setText(name);
    QCOMPARE(updateOkButtonChanged.count(), 1);
    QVERIFY(!updateOkButtonChanged.at(0).at(0).toBool());
    updateOkButtonChanged.clear();
    QVERIFY(!mWarningLabel->isHidden());
}
