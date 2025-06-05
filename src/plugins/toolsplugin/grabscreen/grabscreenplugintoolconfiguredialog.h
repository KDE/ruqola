/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QDialog>
class GrabScreenPluginToolConfigureWidget;
class GrabScreenPluginToolConfigureDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GrabScreenPluginToolConfigureDialog(QWidget *parent = nullptr);
    ~GrabScreenPluginToolConfigureDialog() override;

private:
    GrabScreenPluginToolConfigureWidget *const mGrabScreenPluginToolConfigureWidget;
};
