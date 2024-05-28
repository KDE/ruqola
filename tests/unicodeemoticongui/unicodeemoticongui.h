/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QListWidgetItem>
#include <QWidget>
#include <TextEmoticonsCore/UnicodeEmoticon>

class QLineEdit;
class KListWidgetSearchLine;
class UnicodeEmoticonInfo : public QWidget
{
    Q_OBJECT
public:
    explicit UnicodeEmoticonInfo(QWidget *parent = nullptr);
    ~UnicodeEmoticonInfo() override;

    [[nodiscard]] TextEmoticonsCore::UnicodeEmoticon info() const;
    void setInfo(const TextEmoticonsCore::UnicodeEmoticon &info);

private:
    QLineEdit *mIdentifier = nullptr;
    QLineEdit *mUnicode = nullptr;
    QLineEdit *mAliases = nullptr;
    QLineEdit *mCategory = nullptr;
    QLineEdit *mOrder = nullptr;
    TextEmoticonsCore::UnicodeEmoticon mInfo;
};

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
class UnicodeEmoticonGui : public QWidget
{
    Q_OBJECT
public:
    explicit UnicodeEmoticonGui(QWidget *parent = nullptr);
    ~UnicodeEmoticonGui() override;

private:
    void save();
    void load();
    void slotItemChanged(QListWidgetItem *item);
    void slotItemSelectionChanged();
    void slotExportIdentifier();
    KListWidgetSearchLine *const mSearchEmoticon;
    QListWidget *const mListWidget;
    UnicodeEmoticonInfo *const mWidgetInfo;
};
