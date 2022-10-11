/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

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
    void shouldReturnData();
    void shouldUpdateOnSectionUpdates();

private:
    QStandardItemModel mSourceModel;
};
