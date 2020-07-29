/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#ifndef EMOTICONSELECTORWIDGET_H
#define EMOTICONSELECTORWIDGET_H

#include <QListWidget>
#include "emoticons/unicodeemoticon.h"
#include "libruqolawidgets_private_export.h"

class LIBRUQOLAWIDGETS_TESTS_EXPORT EmoticonTextEditItem : public QListWidgetItem
{
public:
    explicit EmoticonTextEditItem(const QString &identifier, const QString &emoticonText, QListWidget *parent);
    ~EmoticonTextEditItem() override;
    Q_REQUIRED_RESULT QString text() const;
    Q_REQUIRED_RESULT QString identifier() const;

private:
    QString mText;
    QString mIdentifier;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT EmoticonSelectorWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit EmoticonSelectorWidget(QWidget *parent = nullptr);
    ~EmoticonSelectorWidget() override;

    void setEmoticons(const QVector<UnicodeEmoticon> &emoticons);

Q_SIGNALS:
    void itemSelected(const QString &str);
private:
    void slotMouseOverItem(QListWidgetItem *item);
    void slotEmoticonClicked(QListWidgetItem *item);
};

#endif // EMOTICONSELECTORWIDGET_H
