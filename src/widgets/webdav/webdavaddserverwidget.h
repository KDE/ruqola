/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

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
    struct LIBRUQOLAWIDGETS_TESTS_EXPORT WebDavAddServerInfo {
        QString name;
        QString url;
        QString userName;
        QString password;

        [[nodiscard]] bool isValid() const;
    };
    explicit WebDavAddServerWidget(QWidget *parent = nullptr);
    ~WebDavAddServerWidget() override;

    [[nodiscard]] WebDavAddServerInfo addServerInfo() const;

Q_SIGNALS:
    void okButtonEnabled(bool enabled);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUpdateOkButtonStatus();
    QLineEdit *const mName;
    QLineEdit *const mUrl;
    QLineEdit *const mUserName;
    KPasswordLineEdit *const mPasswordLineEdit;
};
