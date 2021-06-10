#include "viewlogwidgettest.h"
#include "administratordialog/logs/viewlogwidget.h"
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
}
