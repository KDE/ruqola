/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
class QLineEdit;
namespace TextAutoCorrection
{
/**
 * @brief The LineEditCatchReturnKey class
 * @author Laurent Montel <montel@kde.org>
 */
class LineEditCatchReturnKey : public QObject
{
    Q_OBJECT
public:
    explicit LineEditCatchReturnKey(QLineEdit *lineEdit, QObject *parent = nullptr);
    ~LineEditCatchReturnKey() override;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QLineEdit *const mLineEdit;
};
}
