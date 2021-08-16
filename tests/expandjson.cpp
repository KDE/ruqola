/*
   Copyright (c) 2017-2021 Laurent Montel <montel@kde.org>

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

#include "expandjson.h"
#include <QApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

ExpandJson::ExpandJson(QWidget *parent)
    : QWidget(parent)
    , mInput(new QTextEdit(this))
    , mOutput(new QTextEdit(this))
    , mExpandButton(new QPushButton(QStringLiteral("Expand Json String"), this))
{
    auto layout = new QVBoxLayout(this);
    mInput->setAcceptRichText(false);
    layout->addWidget(mInput);

    mOutput->setReadOnly(true);
    layout->addWidget(mOutput);

    layout->addWidget(mExpandButton);
    connect(mExpandButton, &QPushButton::clicked, this, &ExpandJson::slotExpandJson);
}

void ExpandJson::slotExpandJson()
{
    const QString inputStr = mInput->toPlainText().trimmed();
    if (!inputStr.isEmpty()) {
        QJsonDocument json = QJsonDocument::fromJson(inputStr.toUtf8());
        qDebug() << " json " << json << inputStr.toUtf8();
        mOutput->setText(QString::fromUtf8(json.toJson(QJsonDocument::Indented)));
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ExpandJson w;
    w.show();
    return app.exec();
}
