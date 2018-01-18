/*
   Copyright (c) 2018 Veluri Mithun  <velurimithun38@gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef QMLTEST_H
#define QMLTEST_H

#include "ruqolaregisterengine.h"

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

class QmlTest : public QWidget
{
    Q_OBJECT

public:
    explicit QmlTest(QWidget *parent = nullptr);
    ~QmlTest();
    void loadQmlFile(const QString &qmlPath);
    void loadCombobox();

private Q_SLOTS:
    void slotPushButtonClicked();

private:
    void addFileToComboBox();
    bool isQmlFile(const QString &filename);

    QComboBox *combobox = nullptr;
    QLabel *FileNameLabel = nullptr;
    QPushButton *loadButton = nullptr;
    RuqolaRegisterEngine *mEngine = nullptr;

};

#endif // QMLTEST_H
