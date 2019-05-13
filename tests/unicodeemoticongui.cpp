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
#include <QFormLayout>
#include <QLineEdit>
#include <KListWidgetSearchLine>

#include <emoticons/unicodeemoticonparser.h>

UnicodeEmoticonGui::UnicodeEmoticonGui(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(hboxLayout);

    QVBoxLayout *mainComponentLayout = new QVBoxLayout;
    mainComponentLayout->setContentsMargins(0, 0, 0, 0);

    mSearchEmoticon = new KListWidgetSearchLine(this);
    mListWidget = new QListWidget(this);
    mSearchEmoticon->setListWidget(mListWidget);
    mainComponentLayout->addWidget(mSearchEmoticon);
    mainComponentLayout->addWidget(mListWidget);
    hboxLayout->addLayout(mainComponentLayout);

    mWidgetInfo = new UnicodeEmoticonInfo(this);
    hboxLayout->addWidget(mWidgetInfo);

    QPushButton *save = new QPushButton(QStringLiteral("Save"), this);
    mainLayout->addWidget(save);
    connect(save, &QPushButton::clicked, this, &UnicodeEmoticonGui::save);

    connect(mListWidget, &QListWidget::itemClicked, this, &UnicodeEmoticonGui::slotItemChanged);
    connect(mListWidget, &QListWidget::itemSelectionChanged, this, &UnicodeEmoticonGui::slotItemSelectionChanged);
    load();
}

UnicodeEmoticonGui::~UnicodeEmoticonGui()
{
}

void UnicodeEmoticonGui::slotItemSelectionChanged()
{
    QListWidgetItem *item = mListWidget->currentItem();
    slotItemChanged(item);
}

void UnicodeEmoticonGui::slotItemChanged(QListWidgetItem *item)
{
    if (item) {
        UnicodeEmoticonListWidgetItem *itemResult = static_cast<UnicodeEmoticonListWidgetItem *>(item);
        UnicodeEmoticon info = itemResult->info();
        mWidgetInfo->setInfo(info);
    }
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
        UnicodeEmoticonListWidgetItem *item = new UnicodeEmoticonListWidgetItem(unicodeEmojiList.at(i).identifier(), mListWidget);
        item->setInfo(unicodeEmojiList.at(i));
        //Allow to update it.
    }
}

void UnicodeEmoticonGui::save()
{
    QJsonDocument doc;
    QJsonObject o;
    doc.setObject(o);
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

UnicodeEmoticonInfo::UnicodeEmoticonInfo(QWidget *parent)
    : QWidget(parent)
{
    QFormLayout *mainLayout = new QFormLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mIdentifier = new QLineEdit(this);
    mainLayout->addRow(QStringLiteral("identifier:"), mIdentifier);
    mUnicode = new QLineEdit(this);
    mainLayout->addRow(QStringLiteral("unicode:"), mUnicode);
    mAliases = new QLineEdit(this);
    mainLayout->addRow(QStringLiteral("aliases:"), mAliases);
    mCategory = new QLineEdit(this);
    mainLayout->addRow(QStringLiteral("category:"), mCategory);
    mOrder = new QLineEdit(this);
    mainLayout->addRow(QStringLiteral("order:"), mOrder);
}

UnicodeEmoticonInfo::~UnicodeEmoticonInfo()
{
}

UnicodeEmoticon UnicodeEmoticonInfo::info() const
{
    return mInfo;
}

void UnicodeEmoticonInfo::setInfo(const UnicodeEmoticon &info)
{
    mIdentifier->setText(info.identifier());
    mUnicode->setText(info.unicode());
    mAliases->setText(info.aliases().join(QLatin1Char(',')));
    mCategory->setText(info.category());
    mOrder->setText(QString::number(info.order()));
    mInfo = info;
}

UnicodeEmoticonListWidgetItem::UnicodeEmoticonListWidgetItem(const QString &str, QListWidget *parent)
    : QListWidgetItem(str, parent)
{
}

UnicodeEmoticon UnicodeEmoticonListWidgetItem::info() const
{
    return mInfo;
}

void UnicodeEmoticonListWidgetItem::setInfo(const UnicodeEmoticon &info)
{
    mInfo = info;
}
