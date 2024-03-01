/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"

class TextSelection;

class LIBRUQOLAWIDGETS_TESTS_EXPORT TextSelectionImpl
{
public:
    TextSelectionImpl();
    ~TextSelectionImpl();

    [[nodiscard]] TextSelection *textSelection() const;

    [[nodiscard]] bool mightStartDrag() const;
    void setMightStartDrag(bool newMightStartDrag);

private:
    bool mMightStartDrag = false;
    TextSelection *const mTextSelection;
};
