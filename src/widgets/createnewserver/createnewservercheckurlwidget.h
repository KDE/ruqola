/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class KMessageWidget;
class KBusyIndicatorWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewServerCheckUrlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateNewServerCheckUrlWidget(QWidget *parent = nullptr);
    ~CreateNewServerCheckUrlWidget() override;

Q_SIGNALS:
    void serverUrlFound(const QString &url);

private:
    void slotTestConnection();
    void slotErrorConnection();
    void slotSuccessConnection();
    QLineEdit *const mServerUrl;
    KBusyIndicatorWidget *const mBusyIndicatorWidget;
    KMessageWidget *const mFailedError;
};
