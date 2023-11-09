/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <KLazyLocalizedString>

// Version 2.0

// enter items for the "Important changes" list here:
static const KLazyLocalizedString ruqolaChangesV2_0[] = {
    kli18n("Store Message in Local Database (experimental)"),
};
static const int numRuqolaChanges2_0 = sizeof ruqolaChangesV2_0 / sizeof *ruqolaChangesV2_0;

// enter items for the "new features" list here, so the main body of
// the welcome page can be left untouched (probably much easier for
// the translators). Note that the <li>...</li> tags are added
// automatically below:
static const KLazyLocalizedString ruqolaNewFeatures2_0[] = {
    kli18n("Download Server Icon from Server"),
    kli18n("Show Server Error Info in Specific DialogBox"),
    kli18n("Allow to Copy Image in Clipboard"),
    kli18n("Improve debug support (Allow to show current account permissions)"),
    kli18n("Add support for quoted text (text which starts by \'>\')"),
    kli18n("Show leader/moderator/owner in channel info."),
    kli18n("Video/Sound Message support (kf6 only)."),
    kli18n("Import/Export Accounts."),
};
static const int numRuqolaNewFeatures2_0 = sizeof ruqolaNewFeatures2_0 / sizeof *ruqolaNewFeatures2_0;

// enter items for the "Important changes" list here:
static const KLazyLocalizedString ruqolaBugfixing2_0[] = {
    kli18n("Fix avatar support"),
    kli18n("Fix emoji support (use new ktextaddons/textemoticons)"),
    kli18n("Fix animated gif in reactions"),
    kli18n("Fix kf6 support"),
};
static const int numRuqolaBugfixing2_0 = sizeof ruqolaBugfixing2_0 / sizeof *ruqolaBugfixing2_0;

// Version 2.1

static const KLazyLocalizedString ruqolaNewFeatures2_1[] = {
    kli18n("Add Moderation Console (Administration, RC server 6.2)"),
};
static const int numRuqolaNewFeatures2_1 = sizeof ruqolaNewFeatures2_1 / sizeof *ruqolaNewFeatures2_1;

// enter items for the "Important changes" list here:
static const KLazyLocalizedString ruqolaBugfixing2_1[] = {
    kli18n("Fix markdown support (header + check mark)"),
    kli18n("Fix mem leak"),
    kli18n("Cache message size (optimization)"),
    kli18n("Improve select room or user in message line edit (show icon, inform if user is in channel)"),
};
static const int numRuqolaBugfixing2_1 = sizeof ruqolaBugfixing2_1 / sizeof *ruqolaBugfixing2_1;
