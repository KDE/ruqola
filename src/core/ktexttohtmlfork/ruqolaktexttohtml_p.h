/*
    SPDX-FileCopyrightText: 2002 Dave Corrie <kde@davecorrie.com>
    SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QString>

class LIBRUQOLACORE_EXPORT KTextToHTMLHelper
{
public:
    KTextToHTMLHelper(const QString &plainText, int pos = 0, int maxUrlLen = 4096, int maxAddressLen = 255);

    QString getEmailAddress();
    QString getPhoneNumber();
    bool atUrl() const;
    bool isEmptyUrl(const QString &url) const;
    QString getUrl(bool *badurl = nullptr);
    QString highlightedText();

    QString mText;
    int mMaxUrlLen;
    int mMaxAddressLen;
    int mPos;
};
