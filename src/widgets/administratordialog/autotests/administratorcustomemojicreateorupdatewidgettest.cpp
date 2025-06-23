/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomemojicreateorupdatewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratordialog/customemoji/administratorcustomemojicreateorupdatewidget.h"

#include <KUrlRequester>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(AdministratorCustomEmojiCreateOrUpdateWidgetTest)
AdministratorCustomEmojiCreateOrUpdateWidgetTest::AdministratorCustomEmojiCreateOrUpdateWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorCustomEmojiCreateOrUpdateWidgetTest::shouldHaveDefaultValues()
{
    AdministratorCustomEmojiCreateOrUpdateWidget w;
    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mName = w.findChild<QLineEdit *>(u"mName"_s);
    QVERIFY(mName);
    QVERIFY(mName->text().isEmpty());
    QVERIFY(mName->isClearButtonEnabled());

    auto mAlias = w.findChild<QLineEdit *>(u"mAlias"_s);
    QVERIFY(mAlias);
    QVERIFY(mAlias->text().isEmpty());
    QVERIFY(mAlias->isClearButtonEnabled());

    auto mSelectFile = w.findChild<KUrlRequester *>(u"mSelectFile"_s);
    QVERIFY(mSelectFile);

    auto mWarningLabel = w.findChild<QLabel *>(u"mWarningLabel"_s);
    QVERIFY(mWarningLabel);
    QVERIFY(mWarningLabel->isHidden());
    QCOMPARE(w.type(), AdministratorCustomEmojiCreateOrUpdateWidget::AdministratorCustomEmojiCreateOrUpdateType::Create);

    auto mIconLabel = w.findChild<QLabel *>(u"mIconLabel"_s);
    QVERIFY(mIconLabel);
    QVERIFY(mIconLabel->text().isEmpty());
}

void AdministratorCustomEmojiCreateOrUpdateWidgetTest::shouldEmitSignal()
{
    AdministratorCustomEmojiCreateOrUpdateWidget w;
    QSignalSpy updateOkButtonChanged(&w, &AdministratorCustomEmojiCreateOrUpdateWidget::updateOkButton);

    auto mName = w.findChild<QLineEdit *>(u"mName"_s);
    auto mAlias = w.findChild<QLineEdit *>(u"mAlias"_s);
    auto mSelectFile = w.findChild<KUrlRequester *>(u"mSelectFile"_s);
    auto mWarningLabel = w.findChild<QLabel *>(u"mWarningLabel"_s);

    const QString name = u"bla"_s;
    mName->setText(name);
    QCOMPARE(updateOkButtonChanged.count(), 1);
    QVERIFY(!updateOkButtonChanged.at(0).at(0).toBool());
    updateOkButtonChanged.clear();
    QVERIFY(mWarningLabel->isHidden());

    mAlias->setText(u"bli"_s);
    QCOMPARE(updateOkButtonChanged.count(), 1);
    QVERIFY(!updateOkButtonChanged.at(0).at(0).toBool());
    updateOkButtonChanged.clear();
    QVERIFY(mWarningLabel->isHidden());

    // Valid
    mSelectFile->setUrl(QUrl::fromLocalFile(u"/tmp/bla"_s));
    QCOMPARE(updateOkButtonChanged.count(), 1);
    QVERIFY(updateOkButtonChanged.at(0).at(0).toBool());
    updateOkButtonChanged.clear();

    mAlias->clear();
    QCOMPARE(updateOkButtonChanged.count(), 1);
    // Always valid when we clear alias
    QVERIFY(updateOkButtonChanged.at(0).at(0).toBool());
    updateOkButtonChanged.clear();

    // Valid
    mSelectFile->setUrl(QUrl::fromLocalFile(u"/tmp/bla2"_s));
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

#include "moc_administratorcustomemojicreateorupdatewidgettest.cpp"
