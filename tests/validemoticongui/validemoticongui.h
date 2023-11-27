/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QListWidgetItem>
#include <QWidget>
#include <TextEmoticonsCore/UnicodeEmoticon>

class QLineEdit;
class KListWidgetSearchLine;

class UnicodeEmoticonListWidgetItem : public QListWidgetItem
{
public:
    explicit UnicodeEmoticonListWidgetItem(const QString &str, QListWidget *parent);

    [[nodiscard]] TextEmoticonsCore::UnicodeEmoticon info() const;
    void setInfo(const TextEmoticonsCore::UnicodeEmoticon &info);

private:
    TextEmoticonsCore::UnicodeEmoticon mInfo;
};

class QListWidget;
class ValidEmoticonGui : public QWidget
{
    Q_OBJECT
public:
    explicit ValidEmoticonGui(QWidget *parent = nullptr);
    ~ValidEmoticonGui() override;

private:
    [[nodiscard]] QStringList load();
    [[nodiscard]] QString generateExcludeEmoticon(const QUrl &url, const QStringList &identifiers) const;
    QListWidget *mListWidget = nullptr;
    KListWidgetSearchLine *mSearchEmoticon = nullptr;
};
