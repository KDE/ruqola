#pragma once

#include <QObject>

class MyAccountPersonalAccessTokenTreeViewTest : public QObject
{
    Q_OBJECT
public:
    explicit MyAccountPersonalAccessTokenTreeViewTest(QObject *parent = nullptr);
    ~MyAccountPersonalAccessTokenTreeViewTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
