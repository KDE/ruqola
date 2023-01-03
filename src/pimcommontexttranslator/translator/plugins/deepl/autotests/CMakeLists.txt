# SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
ecm_add_test(deeplengineclienttest.cpp deeplengineclienttest.h ../deeplengineclient.cpp ../deeplengineplugin.cpp ../deeplengineutil.cpp ${translator_debug_deepl_SRCS}
    ../deeplengineconfiguredialog.cpp
    ../deeplengineconfigurewidget.cpp
    TEST_NAME deeplengineclienttest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n KF5::ConfigCore
)

#####
ecm_add_test(deeplengineconfiguredialogtest.cpp deeplengineconfiguredialogtest.h ../deeplengineconfiguredialog.cpp ../deeplengineconfigurewidget.cpp ${translator_debug_deepl_SRCS}
    TEST_NAME deeplengineconfiguredialogtest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n KF5::ConfigCore
)

#####
ecm_add_test(deeplengineconfigurewidgettest.cpp deeplengineconfigurewidgettest.h ../deeplengineconfigurewidget.cpp
    TEST_NAME deeplengineconfigurewidgettest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n KF5::ConfigCore
)
