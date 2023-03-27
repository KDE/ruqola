/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "config-ruqola.h"
#include "libruqolawidgets_private_export.h"
#if HAVE_TEXT_ADDONS_WIDGETS_SUPPORT
#include <TextAddonsWidgets/LineEditCatchReturnKey>
using namespace TextAddonsWidgets;
#else
#include <QObject>
class QLineEdit;
/**
 * @brief The LineEditCatchReturnKey class
 * @author Laurent Montel <montel@kde.org>
 */
class LIBRUQOLAWIDGETS_TESTS_EXPORT LineEditCatchReturnKey : public QObject
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
#endif
