/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "validemoticongui.h"

#include <KListWidgetSearchLine>
#include <KUrlRequester>

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRegularExpression>
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
    urlRequester->setPlaceholderText(QStringLiteral("<foo>/packages/livechat/src/components/Emoji/emojis.ts"));
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
    QFile file(url.toLocalFile());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return {};
    }
    QStringList resultRocketChatEmoticons;
    QTextStream in(&file);
    while (!in.atEnd()) {
        const QString line = in.readLine();
        static QRegularExpression regular{QStringLiteral("':(.*):':")};
        QRegularExpressionMatch match;
        if (line.contains(regular, &match)) {
            const QString captured = match.captured(1);
            resultRocketChatEmoticons.append(QStringLiteral(":%1:").arg(captured));
        }
    }
    QString result;
    for (const auto &emoticons : identifiers) {
        if (!resultRocketChatEmoticons.contains(emoticons)) {
            result.append(QStringLiteral("QStringLiteral(\"%1\"),\n").arg(emoticons));
        }
    }

    return result;
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

#include "moc_validemoticongui.cpp"
