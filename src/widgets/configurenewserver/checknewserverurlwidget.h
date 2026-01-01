/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "job/extractserverinfojob.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class KMessageWidget;
class KBusyIndicatorWidget;
class QPushButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CheckNewServerUrlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CheckNewServerUrlWidget(QWidget *parent = nullptr);
    ~CheckNewServerUrlWidget() override;

    void setServerUrl(const QString &serverUrl);

Q_SIGNALS:
    void serverUrlFound(const ExtractServerInfoJob::ServerInfo &info);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTestConnection();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotErrorConnection(const QString &message);
    QLineEdit *const mServerUrl;
    KBusyIndicatorWidget *const mBusyIndicatorWidget;
    KMessageWidget *const mFailedError;
    QPushButton *const mConnectionPushButton;
};
