/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include <QWidget>
class QLineEdit;
class WebDavAddServerWidget : public QWidget
{
    Q_OBJECT
public:
    struct WebDavAddServerInfo {
        QString name;
        QString url;
        QString userName;
        QString password;
    };
    explicit WebDavAddServerWidget(QWidget *parent = nullptr);
    ~WebDavAddServerWidget() override;

    [[nodiscard]] WebDavAddServerInfo addServerInfo() const;

private:
    QLineEdit *const mName;
    QLineEdit *const mUrl;
    QLineEdit *const mUserName;
};
