/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QObject>

class TextSelection;

class LIBRUQOLAWIDGETS_TESTS_EXPORT TextSelectionImpl
{
public:
    TextSelectionImpl();
    ~TextSelectionImpl();

    Q_REQUIRED_RESULT TextSelection *textSelection() const;

    Q_REQUIRED_RESULT bool mightStartDrag() const;
    void setMightStartDrag(bool newMightStartDrag);

private:
    bool mMightStartDrag = false;
    TextSelection *const mTextSelection;
};
