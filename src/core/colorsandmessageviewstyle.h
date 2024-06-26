/*
   SPDX-FileCopyrightText: 2022 Nicolas Fella <nicolas.fella@gmx.de>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

#include <KColorScheme>

#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT ColorsAndMessageViewStyle : public QObject
{
    Q_OBJECT
public:
    static ColorsAndMessageViewStyle &self();

    ColorsAndMessageViewStyle();

    [[nodiscard]] KColorScheme schemeView() const;

    [[nodiscard]] KColorScheme schemeWindow() const;

Q_SIGNALS:
    void needToUpdateColors();
    void needUpdateMessageStyle();
    void needUpdateFontSize();

protected:
    [[nodiscard]] bool eventFilter(QObject *obj, QEvent *event) override;

private:
    LIBRUQOLACORE_NO_EXPORT void regenerateColorScheme();
    KColorScheme mSchemeView;
    KColorScheme mSchemeWindow;
};
