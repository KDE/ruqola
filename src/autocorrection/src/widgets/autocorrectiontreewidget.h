/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QTreeWidget>

namespace TextAutoCorrection
{
class AutoCorrectionTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit AutoCorrectionTreeWidget(QWidget *parent = nullptr);
    ~AutoCorrectionTreeWidget() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

Q_SIGNALS:
    void deleteSelectedItems();
};
}
