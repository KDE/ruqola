/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "validemoticongui.h"

#include <KListWidgetSearchLine>
#include <KUrlRequester>

#include <QApplication>
#include <QFile>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPlainTextEdit>
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

    auto hboxLayoutUrlRequester = new QHBoxLayout;
    vboxLayout->addLayout(hboxLayoutUrlRequester);

    auto label = new QLabel(QStringLiteral("Path emoji RocketChat:"), this);
    hboxLayoutUrlRequester->addWidget(label);

    auto urlRequester = new KUrlRequester(this);
    hboxLayoutUrlRequester->addWidget(urlRequester);

    auto plainTextEdit = new QPlainTextEdit(this);
    plainTextEdit->setReadOnly(true);
    vboxLayout->addWidget(plainTextEdit);

    const QStringList identifiers = load();
    connect(urlRequester, &KUrlRequester::urlSelected, this, [this, plainTextEdit, identifiers](const QUrl &url) {
        if (url.isValid()) {
            plainTextEdit->setPlainText(generateExcludeEmoticon(url, identifiers));
        }
    });
}

ValidEmoticonGui::~ValidEmoticonGui() = default;

QString ValidEmoticonGui::generateExcludeEmoticon(const QUrl &url, const QStringList &identifiers) const
{
    // TODO parse file

    // TODO generate exclude emoticons list
    return {};
}

QStringList ValidEmoticonGui::load()
{
    TextEmoticonsCore::UnicodeEmoticonParser unicodeParser;
    QFile file(QStringLiteral(":/emoji.json"));
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Impossible to open file: " << file.errorString();
        return {};
    }
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    QStringList lst;
    const QJsonObject obj = doc.object();
    const QList<TextEmoticonsCore::UnicodeEmoticon> unicodeEmojiList = unicodeParser.parse(obj);
    for (const TextEmoticonsCore::UnicodeEmoticon &emoticon : unicodeEmojiList) {
        const auto identifier = emoticon.identifier();
        auto item = new UnicodeEmoticonListWidgetItem(identifier, mListWidget);
        item->setInfo(emoticon);
        lst << identifier;
    }
    return lst;
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
