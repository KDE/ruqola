# SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
ecm_add_test(bingengineclienttest.cpp bingengineclienttest.h ../bingengineclient.cpp ../bingengineplugin.cpp ${translator_debug_bing_SRCS}
    TEST_NAME bingengineclienttest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n
)
