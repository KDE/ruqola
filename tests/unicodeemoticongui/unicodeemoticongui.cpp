/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "unicodeemoticongui.h"
using namespace Qt::Literals::StringLiterals;

#include <KListWidgetSearchLine>
#include <QApplication>
#include <QFile>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>
#include <TextEmoticonsCore/UnicodeEmoticonParser>

UnicodeEmoticonGui::UnicodeEmoticonGui(QWidget *parent)
    : QWidget(parent)
    , mSearchEmoticon(new KListWidgetSearchLine(this))
    , mListWidget(new QListWidget(this))
    , mWidgetInfo(new UnicodeEmoticonInfo(this))
{
    auto mainLayout = new QVBoxLayout(this);
    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    auto mainComponentLayout = new QVBoxLayout;
    mainComponentLayout->setContentsMargins({});

    mSearchEmoticon->setListWidget(mListWidget);
    mainComponentLayout->addWidget(mSearchEmoticon);
    mainComponentLayout->addWidget(mListWidget);
    hboxLayout->addLayout(mainComponentLayout);

    hboxLayout->addWidget(mWidgetInfo);

    auto buttonLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonLayout);
    auto savePushButton = new QPushButton(u"Save…"_s, this);
    buttonLayout->addWidget(savePushButton);
    connect(savePushButton, &QPushButton::clicked, this, &UnicodeEmoticonGui::save);

    auto exportIdentifier = new QPushButton(u"Export identifiers…"_s, this);
    buttonLayout->addWidget(exportIdentifier);
    connect(exportIdentifier, &QPushButton::clicked, this, &UnicodeEmoticonGui::slotExportIdentifier);

    connect(mListWidget, &QListWidget::itemClicked, this, &UnicodeEmoticonGui::slotItemChanged);
    connect(mListWidget, &QListWidget::itemSelectionChanged, this, &UnicodeEmoticonGui::slotItemSelectionChanged);
    load();
}

UnicodeEmoticonGui::~UnicodeEmoticonGui() = default;

void UnicodeEmoticonGui::slotItemSelectionChanged()
{
    QListWidgetItem *item = mListWidget->currentItem();
    slotItemChanged(item);
}

void UnicodeEmoticonGui::slotItemChanged(QListWidgetItem *item)
{
    if (item) {
        const auto itemResult = static_cast<UnicodeEmoticonListWidgetItem *>(item);
        TextEmoticonsCore::UnicodeEmoticon info = itemResult->info();
        mWidgetInfo->setInfo(info);
    }
}

void UnicodeEmoticonGui::load()
{
    TextEmoticonsCore::UnicodeEmoticonParser unicodeParser;
    QFile file(u":/emoji.json"_s);
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

void UnicodeEmoticonGui::save()
{
    QJsonDocument doc;
    QJsonObject o;
    doc.setObject(o);
    // TODO
}

void UnicodeEmoticonGui::slotExportIdentifier()
{
    // TODO
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
    auto mainLayout = new QFormLayout(this);
    mainLayout->setContentsMargins({});
    mIdentifier = new QLineEdit(this);
    mainLayout->addRow(u"identifier:"_s, mIdentifier);
    mUnicode = new QLineEdit(this);
    mainLayout->addRow(u"unicode:"_s, mUnicode);
    mAliases = new QLineEdit(this);
    mainLayout->addRow(u"aliases:"_s, mAliases);
    mCategory = new QLineEdit(this);
    mainLayout->addRow(u"category:"_s, mCategory);
    mOrder = new QLineEdit(this);
    mainLayout->addRow(u"order:"_s, mOrder);
}

UnicodeEmoticonInfo::~UnicodeEmoticonInfo() = default;

TextEmoticonsCore::UnicodeEmoticon UnicodeEmoticonInfo::info() const
{
    return mInfo;
}

void UnicodeEmoticonInfo::setInfo(const TextEmoticonsCore::UnicodeEmoticon &info)
{
    mIdentifier->setText(info.identifier());
    mUnicode->setText(info.unicode());
    mAliases->setText(info.aliases().join(u','));
    mCategory->setText(info.category());
    mOrder->setText(QString::number(info.order()));
    mInfo = info;
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

#include "moc_unicodeemoticongui.cpp"
