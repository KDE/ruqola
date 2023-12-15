/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class QLineEdit;
class CreateNewServerCheckUrlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateNewServerCheckUrlWidget(QWidget *parent = nullptr);
    ~CreateNewServerCheckUrlWidget() override;

private:
    void slotTestConnection();
    QLineEdit *const mServerUrl;
};
