/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessagedialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "forwardmessage/forwardmessagedialog.h"
#include "forwardmessage/forwardmessagewidget.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ForwardMessageDialogTest)
ForwardMessageDialogTest::ForwardMessageDialogTest(QObject *parent)
    : QObject{parent}
{
}

void ForwardMessageDialogTest::shouldHaveDefaultValues()
{
    ForwardMessageDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mForwardMessageWidget = d.findChild<ForwardMessageWidget *>(u"mForwardMessageWidget"_s);
    QVERIFY(mForwardMessageWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    QPushButton *buttonOk = button->button(QDialogButtonBox::Ok);
    QVERIFY(buttonOk);
    QVERIFY(!buttonOk->isEnabled());

    QVERIFY(d.channelIdentifiers().isEmpty());
}

#include "moc_forwardmessagedialogtest.cpp"
