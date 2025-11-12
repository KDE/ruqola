/*
   SPDX-FileCopyrightText: 2022 Nicolas Fella <nicolas.fella@gmx.de>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "colorsandmessageviewstyle.h"

#include <QApplication>

ColorsAndMessageViewStyle::ColorsAndMessageViewStyle()
{
}

ColorsAndMessageViewStyle &ColorsAndMessageViewStyle::self()
{
    static ColorsAndMessageViewStyle c;
    return c;
}

#include "moc_colorsandmessageviewstyle.cpp"
