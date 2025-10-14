/*
   SPDX-FileCopyrightText: 2022 Nicolas Fella <nicolas.fella@gmx.de>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "config-ruqola.h"
#if HAVE_TEXTUTILS_COLOR_AND_MESSAGE_VIEW_STYLE_SUPPORT
#include <TextUtils/TextUtilsColorsAndMessageViewStyle>
#endif
#include <QObject>

#include <KColorScheme>

#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT ColorsAndMessageViewStyle :
#if HAVE_TEXTUTILS_COLOR_AND_MESSAGE_VIEW_STYLE_SUPPORT
    public TextUtils::TextUtilsColorsAndMessageViewStyle
#else
    public QObject
#endif
{
    Q_OBJECT
public:
    static ColorsAndMessageViewStyle &self();

    ColorsAndMessageViewStyle();
#if !HAVE_TEXTUTILS_COLOR_AND_MESSAGE_VIEW_STYLE_SUPPORT
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
#endif
};
