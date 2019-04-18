/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#include "unicodeemoticongui.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QStandardPaths>
#include <QPushButton>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QListWidget>

#include <emoticons/unicodeemoticonparser.h>

UnicodeEmoticonGui::UnicodeEmoticonGui(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mListWidget = new QListWidget(this);
    mainLayout->addWidget(mListWidget);

    load();
}

UnicodeEmoticonGui::~UnicodeEmoticonGui()
{
}

void UnicodeEmoticonGui::load()
{
    UnicodeEmoticonParser unicodeParser;
    QFile file(QStringLiteral(":/emoji.json"));
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Impossible to open file: " << file.errorString();
        return;
    }
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    const QJsonObject obj = doc.object();
    const QVector<UnicodeEmoticon> unicodeEmojiList = unicodeParser.parse(obj);
    for (int i = 0; i < unicodeEmojiList.count(); ++i) {
        new QListWidgetItem(unicodeEmojiList.at(i).identifier(), mListWidget);
    }
}

void UnicodeEmoticonGui::save()
{
    //TODO
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    UnicodeEmoticonGui w;
    w.show();
    return app.exec();
}

