/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
namespace TextAutoGenerateTextMcpProtocolWidgets
{
class McpProtocolServerConfigureWidget;
}
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureServerMcpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureServerMcpWidget(QWidget *parent = nullptr);
    ~ConfigureServerMcpWidget() override;

    void save();
    void load();
    void restoreToDefaults();

private:
    TextAutoGenerateTextMcpProtocolWidgets::McpProtocolServerConfigureWidget *const mMcpProtocolServerConfigureWidget;
    QCheckBox *const mEnabled;
};
