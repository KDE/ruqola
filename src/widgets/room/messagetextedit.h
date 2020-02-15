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

#ifndef MESSAGETEXTEDIT_H
#define MESSAGETEXTEDIT_H

#include "common/completiontextedit.h"

#include "libruqolawidgets_private_export.h"

/**
 * @brief The MessageTextEdit class is the widget used for typing messages to be sent.
 */
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageTextEdit : public CompletionTextEdit
{
    Q_OBJECT
public:
    explicit MessageTextEdit(QWidget *parent = nullptr);
    ~MessageTextEdit() override;

    void insert(const QString &text);
    QString text() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

Q_SIGNALS:
    void sendMessage(const QString &str);
    void keyPressed(QKeyEvent *ev);
    void textEditing(bool clearNotification);

protected:
    void keyPressEvent(QKeyEvent *e) override;

private:
    void slotTextChanged();
    void slotCompletionAvailable();
    void slotComplete(const QModelIndex &index);
};

#endif // MESSAGETEXTEDIT_H
