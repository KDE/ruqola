/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class KPasswordLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT WebDavAddServerWidget : public QWidget
{
    Q_OBJECT
public:
    struct WebDavAddServerInfo {
        QString name;
        QString url;
        QString userName;
        QString password;

        [[nodiscard]] bool isValid() const;
    };
    explicit WebDavAddServerWidget(QWidget *parent = nullptr);
    ~WebDavAddServerWidget() override;

    [[nodiscard]] WebDavAddServerInfo addServerInfo() const;

private:
    QLineEdit *const mName;
    QLineEdit *const mUrl;
    QLineEdit *const mUserName;
    KPasswordLineEdit *const mPasswordLineEdit;
};
