/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QListWidgetItem>
#include <QWidget>

#include <emoticons/unicodeemoticon.h>

class QLineEdit;
class KListWidgetSearchLine;
class UnicodeEmoticonInfo : public QWidget
{
    Q_OBJECT
public:
    explicit UnicodeEmoticonInfo(QWidget *parent = nullptr);
    ~UnicodeEmoticonInfo() override;

    Q_REQUIRED_RESULT UnicodeEmoticon info() const;
    void setInfo(const UnicodeEmoticon &info);

private:
    QLineEdit *mIdentifier = nullptr;
    QLineEdit *mUnicode = nullptr;
    QLineEdit *mAliases = nullptr;
    QLineEdit *mCategory = nullptr;
    QLineEdit *mOrder = nullptr;
    UnicodeEmoticon mInfo;
};

class UnicodeEmoticonListWidgetItem : public QListWidgetItem
{
public:
    explicit UnicodeEmoticonListWidgetItem(const QString &str, QListWidget *parent);

    Q_REQUIRED_RESULT UnicodeEmoticon info() const;
    void setInfo(const UnicodeEmoticon &info);

private:
    UnicodeEmoticon mInfo;
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
    QListWidget *mListWidget = nullptr;
    UnicodeEmoticonInfo *mWidgetInfo = nullptr;
    KListWidgetSearchLine *mSearchEmoticon = nullptr;
};
