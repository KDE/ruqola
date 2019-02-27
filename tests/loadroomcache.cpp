/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#include "loadroomcache.h"

#include <QApplication>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <KUrlRequester>
#include <QJsonDocument>
#include <QJsonObject>

LoadRoomCache::LoadRoomCache(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mCacheTextEdit = new QTextEdit(this);
    mCacheTextEdit->setReadOnly(true);
    mainLayout->addWidget(mCacheTextEdit);

    QHBoxLayout *hbox = new QHBoxLayout;
    mainLayout->addLayout(hbox);
    hbox->setContentsMargins(0, 0, 0, 0);

    QLabel *lab = new QLabel(QStringLiteral("Select cache file:"), this);
    hbox->addWidget(lab);

    mRequester = new KUrlRequester(this);
    hbox->addWidget(mRequester);
    mRequester->setMode(KFile::File);

    QPushButton *openButton = new QPushButton(QStringLiteral("Open"));
    hbox->addWidget(openButton);
    connect(openButton, &QPushButton::clicked, this, &LoadRoomCache::slotOpenFile);
}

void LoadRoomCache::slotOpenFile()
{
    mCacheTextEdit->clear();
    if (mRequester->url().isValid() && mRequester->url().isLocalFile()) {
        QFile f;
        f.setFileName(mRequester->url().path());
        if (f.open(QIODevice::ReadOnly)) {
            QDataStream in(&f);
            while (!f.atEnd()) {
                char *byteArray;
                quint32 length;
                in.readBytes(byteArray, length);
                const QByteArray arr = QByteArray::fromRawData(byteArray, length);
                const QByteArray expandJSon = QJsonDocument::fromBinaryData(arr).toJson();
                mCacheTextEdit->append(QString::fromUtf8(expandJSon));
            }
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    LoadRoomCache w;
    w.show();
    return app.exec();
}
