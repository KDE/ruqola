/*
   SPDX-FileCopyrightText: 2022 Nicolas Fella <nicolas.fella@gmx.de>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

#include <KColorScheme>

#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT Colors : public QObject
{
    Q_OBJECT
public:
    static Colors &self();

    Colors();

    [[nodiscard]] KColorScheme schemeView() const;

    [[nodiscard]] KColorScheme schemeWindow() const;

Q_SIGNALS:
    void needToUpdateColors();

protected:
    bool event(QEvent *e) override;

private:
    void regenerateColorScheme();
    KColorScheme mSchemeView;
    KColorScheme mSchemeWindow;
};
