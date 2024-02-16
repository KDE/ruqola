/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "authenticationinfo.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class KMessageWidget;
class KBusyIndicatorWidget;
class QPushButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewServerCheckUrlWidget : public QWidget
{
    Q_OBJECT
public:
    struct LIBRUQOLAWIDGETS_TESTS_EXPORT ServerInfo {
        QString url;
        QList<AuthenticationInfo> authenticationInfos;
        bool canResetPassword = false;
        bool canRegisterAccount = false;
    };

    explicit CreateNewServerCheckUrlWidget(QWidget *parent = nullptr);
    ~CreateNewServerCheckUrlWidget() override;

Q_SIGNALS:
    void serverUrlFound(const CreateNewServerCheckUrlWidget::ServerInfo &info);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTestConnection();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotErrorConnection();
    QLineEdit *const mServerUrl;
    KBusyIndicatorWidget *const mBusyIndicatorWidget;
    KMessageWidget *const mFailedError;
    QPushButton *const mConnectionPushButton;
};
Q_DECLARE_TYPEINFO(CreateNewServerCheckUrlWidget::ServerInfo, Q_RELOCATABLE_TYPE);
