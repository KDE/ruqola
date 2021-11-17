#pragma once

#include <QObject>
#include <QStandardItemModel>

class RoomListHeadingsProxyModelTest : public QObject
{
    Q_OBJECT
public:
    explicit RoomListHeadingsProxyModelTest(QObject *parent = nullptr);

private Q_SLOTS:
    void initTestCase();
    void shouldBeEmptyByDefault();
    void shouldReturnRowCount();
    void shouldMapProxyRows_data();
    void shouldMapProxyRows();
    void shouldMapSourceRows_data();
    void shouldMapSourceRows();
    void shouldDetermineProxyRowSection();
    void shouldReturnData();
    void shouldUpdateOnSectionUpdates();
    void shouldWorkOnTopOfQSFPM();

private:
    QStandardItemModel mSourceModel;
};
