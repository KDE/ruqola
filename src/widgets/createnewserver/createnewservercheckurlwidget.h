/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class QLineEdit;
class QLabel;
class KBusyIndicatorWidget;
class CreateNewServerCheckUrlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateNewServerCheckUrlWidget(QWidget *parent = nullptr);
    ~CreateNewServerCheckUrlWidget() override;

Q_SIGNALS:
    void serverUrlFound();

private:
    void slotTestConnection();
    QLineEdit *const mServerUrl;
    KBusyIndicatorWidget *const mBusyIndicatorWidget;
    QLabel *const mFailedError;
};
