#include "myaccountpersonalaccesstokentreeviewtest.h"
#include "myaccount/myaccountpersonalaccesstokentreeview.h"
#include <QTest>
QTEST_MAIN(MyAccountPersonalAccessTokenTreeViewTest)
MyAccountPersonalAccessTokenTreeViewTest::MyAccountPersonalAccessTokenTreeViewTest(QObject *parent)
    : QObject{parent}
{
}

void MyAccountPersonalAccessTokenTreeViewTest::shouldHaveDefaultValues()
{
    MyAccountPersonalAccessTokenTreeView w(nullptr);
    QVERIFY(w.alternatingRowColors());
    QCOMPARE(w.selectionMode(), QAbstractItemView::SingleSelection);
    QCOMPARE(w.selectionBehavior(), QAbstractItemView::SelectRows);
    QVERIFY(!w.rootIsDecorated());
    QVERIFY(w.isSortingEnabled());
    QCOMPARE(w.contextMenuPolicy(), Qt::CustomContextMenu);
}
