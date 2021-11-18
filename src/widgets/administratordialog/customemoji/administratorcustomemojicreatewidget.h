/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDebug>
#include <QUrl>
#include <QWidget>
class QLineEdit;
class KUrlRequester;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorCustomEmojiCreateWidget : public QWidget
{
    Q_OBJECT
public:
    struct CustomEmojiCreateInfo {
        QString alias;
        QString name;
        QUrl fileNameUrl;
    };
    explicit AdministratorCustomEmojiCreateWidget(QWidget *parent = nullptr);
    ~AdministratorCustomEmojiCreateWidget() override;

    void setCustomEmojiInfo(const CustomEmojiCreateInfo &info);

    Q_REQUIRED_RESULT AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo info() const;

Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    void slotUpdateOkButton();
    QLineEdit *const mName;
    QLineEdit *const mAlias;
    KUrlRequester *const mSelectFile;
};
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo &t);
