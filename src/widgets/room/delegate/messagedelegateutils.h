/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QString>
#include <QTextDocument>
#include <memory>
namespace MessageDelegateUtils
{
std::unique_ptr<QTextDocument> createTextDocument(bool useItalic, const QString &text, int width);
}

