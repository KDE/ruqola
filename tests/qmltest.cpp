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

#include "qmltest.h"

#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QFileDialog>
#include <QQuickView>
#include <QHBoxLayout>
#include <QInputDialog>

QmlTest::QmlTest(QWidget *parent)
    : QWidget(parent)
{
    combobox = new QComboBox(this);
    FileNameLabel = new QLabel(QStringLiteral("File name:"), this);
    loadButton = new QPushButton(QStringLiteral("load"), this);
    mEngine = new RuqolaRegisterEngine;

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(FileNameLabel);
    layout->addWidget(combobox);
    layout->addWidget(loadButton);

    connect(loadButton, &QPushButton::clicked, this, &QmlTest::slotPushButtonClicked);

    loadCombobox();//mEngine initialized here
}

QmlTest::~QmlTest()
{
    delete combobox;
    delete FileNameLabel;
    delete loadButton;
    delete mEngine;
}

bool QmlTest::isQmlFile(const QString &filename)
{
    return filename.endsWith(QStringLiteral(".qml"));
}

void QmlTest::loadCombobox()
{
    if (mEngine->initialize()) {
        combobox->addItem(QStringLiteral("Add file"), QString());

        QDir rsrc = QDir(QStringLiteral(":/"));
        QDirIterator dirItr(rsrc);
        while (dirItr.hasNext()) {
            const QString filepath = dirItr.next();
            const QString filename = dirItr.fileName();
            if (isQmlFile(filename)) {
                combobox->addItem(filename, filepath);
            }
        }
    } else {
        //qCWarning(RUQOLA_LOG) << "Unable to initialize RuqolaRegisterEngine";
    }
}

void QmlTest::addFileToComboBox()
{
    //take input from user
    const QString filepath = QFileDialog::getOpenFileName(this, QStringLiteral("Open qml file"),
                                                          QStringLiteral(":/"),
                                                          QStringLiteral("QML Files (*.qml)"));

    const bool notFound = (combobox->findText(filepath) == -1);
    if (notFound && !filepath.isEmpty()) {
        //since user may select other file which is not there in ":/" it is better to show whole path
        combobox->addItem(filepath, filepath);
    }
    //else statment warn the user that comboBox already has that file
}

void QmlTest::loadQmlFile(const QString &qmlPath)
{
    QQuickView *view = new QQuickView(QUrl::fromLocalFile(qmlPath));
    view->show();
}

void QmlTest::slotPushButtonClicked()
{
    QString msg = combobox->currentData(Qt::UserRole).toString();
    if (msg.isEmpty()) {
        addFileToComboBox();
    } else {
        //you can remove this else statment, to load the file directly after taking input from dialog box
        loadQmlFile(msg);
    }
}

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qml);

    QApplication a(argc, argv);
    QmlTest w;
    w.show();
    return a.exec();
}
