/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class UploadFileProgressStatusListWidget;
class UploadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UploadWidget(QWidget *parent = nullptr);
    ~UploadWidget() override = default;

private:
    void addButton();
    UploadFileProgressStatusListWidget *const mUploadFileProgressStatusListWidget;
    static int identifier;
};
