/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "validemoticongui.h"

#include <KListWidgetSearchLine>
#include <QApplication>
#include <QFile>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>
#include <TextEmoticonsCore/UnicodeEmoticonParser>

ValidEmoticonGui::ValidEmoticonGui(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    auto mainComponentLayout = new QVBoxLayout;
    mainComponentLayout->setContentsMargins({});

    mSearchEmoticon = new KListWidgetSearchLine(this);
    mListWidget = new QListWidget(this);
    mSearchEmoticon->setListWidget(mListWidget);
    mainComponentLayout->addWidget(mSearchEmoticon);
    mainComponentLayout->addWidget(mListWidget);
    hboxLayout->addLayout(mainComponentLayout);

    auto vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins({});
    mainLayout->addLayout(vboxLayout);

    // auto buttonLayout = new QHBoxLayout;
    // mainLayout->addLayout(buttonLayout);
    // auto save = new QPushButton(QStringLiteral("Save..."), this);
    // buttonLayout->addWidget(save);
    // connect(save, &QPushButton::clicked, this, &ValidEmoticonGui::save);

    // auto exportIdentifier = new QPushButton(QStringLiteral("Export identifiers..."), this);
    // buttonLayout->addWidget(exportIdentifier);
    // connect(exportIdentifier, &QPushButton::clicked, this, &ValidEmoticonGui::slotExportIdentifier);

    load();
}

ValidEmoticonGui::~ValidEmoticonGui() = default;

void ValidEmoticonGui::load()
{
    TextEmoticonsCore::UnicodeEmoticonParser unicodeParser;
    QFile file(QStringLiteral(":/emoji.json"));
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Impossible to open file: " << file.errorString();
        return;
    }
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    const QJsonObject obj = doc.object();
    const QList<TextEmoticonsCore::UnicodeEmoticon> unicodeEmojiList = unicodeParser.parse(obj);
    for (const TextEmoticonsCore::UnicodeEmoticon &emoticon : unicodeEmojiList) {
        auto item = new UnicodeEmoticonListWidgetItem(emoticon.identifier(), mListWidget);
        item->setInfo(emoticon);
    }
}

UnicodeEmoticonListWidgetItem::UnicodeEmoticonListWidgetItem(const QString &str, QListWidget *parent)
    : QListWidgetItem(str, parent)
{
}

TextEmoticonsCore::UnicodeEmoticon UnicodeEmoticonListWidgetItem::info() const
{
    return mInfo;
}

void UnicodeEmoticonListWidgetItem::setInfo(const TextEmoticonsCore::UnicodeEmoticon &info)
{
    mInfo = info;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    ValidEmoticonGui w;
    w.show();
    return app.exec();
}
