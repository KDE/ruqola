#pragma once

#include <QObject>

class ViewLogWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ViewLogWidgetTest(QObject *parent = nullptr);
    ~ViewLogWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
