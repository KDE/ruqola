/*
   SPDX-FileCopyrightText: 2017-2023 Montel Laurent <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "loadroomcache.h"

#include <KUrlRequester>
#include <QApplication>
#include <QJsonDocument>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>

LoadRoomCache::LoadRoomCache(QWidget *parent)
    : QWidget(parent)
    , mCacheTextEdit(new QTextEdit(this))
    , mRequester(new KUrlRequester(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mCacheTextEdit->setReadOnly(true);
    mainLayout->addWidget(mCacheTextEdit);

    auto hbox = new QHBoxLayout;
    mainLayout->addLayout(hbox);
    hbox->setContentsMargins({});

    auto lab = new QLabel(QStringLiteral("Select cache file:"), this);
    hbox->addWidget(lab);

    hbox->addWidget(mRequester);
    mRequester->setMode(KFile::File);

    auto openButton = new QPushButton(QStringLiteral("Open"));
    hbox->addWidget(openButton);
    connect(openButton, &QPushButton::clicked, this, &LoadRoomCache::slotOpenFile);
}

void LoadRoomCache::slotOpenFile()
{
    mCacheTextEdit->clear();
#if 0 // TODO
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
#endif
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    LoadRoomCache w;
    w.show();
    return app.exec();
}
