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

#ifndef UNICODEEMOTICONGUI_H
#define UNICODEEMOTICONGUI_H

#include <QListWidgetItem>
#include <QWidget>

#include <emoticons/unicodeemoticon.h>

class QLineEdit;
class UnicodeEmoticonInfo : public QWidget
{
    Q_OBJECT
public:
    explicit UnicodeEmoticonInfo(QWidget *parent = nullptr);
    ~UnicodeEmoticonInfo();

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
    ~UnicodeEmoticonGui();
private:
    void save();
    void load();
    void slotItemChanged(QListWidgetItem *item);
    QListWidget *mListWidget = nullptr;
    UnicodeEmoticonInfo *mWidgetInfo = nullptr;
};

#endif // UnicodeEmoticonGui_H
