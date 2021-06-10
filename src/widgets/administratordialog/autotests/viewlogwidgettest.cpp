#include "viewlogwidgettest.h"
#include "administratordialog/logs/viewlogwidget.h"
#include <QPlainTextEdit>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ViewLogWidgetTest)

ViewLogWidgetTest::ViewLogWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ViewLogWidgetTest::shouldHaveDefaultValues()
{
    ViewLogWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto *mPlainTextEdit = w.findChild<QPlainTextEdit *>(QStringLiteral("mPlainTextEdit"));
    QVERIFY(mPlainTextEdit);
    QVERIFY(mPlainTextEdit->isReadOnly());
}
