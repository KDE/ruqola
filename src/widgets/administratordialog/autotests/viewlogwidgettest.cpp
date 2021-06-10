#include "viewlogwidgettest.h"
#include "administratordialog/logs/viewlogwidget.h"
#include <QTest>

QTEST_MAIN(ViewLogWidgetTest)

ViewLogWidgetTest::ViewLogWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ViewLogWidgetTest::shouldHaveDefaultValues()
{
    ViewLogWidget w;
    // TODO
}
