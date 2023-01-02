# SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
ecm_add_test(libretranslateengineclienttest.cpp libretranslateengineclienttest.h ../libretranslateengineclient.cpp ../libretranslateengineplugin.cpp
    ../libretranslateengineconfiguredialog.cpp ../libretranslateengineconfigurewidget.cpp ../libretranslateengineutil.h ../libretranslateengineutil.cpp
    ${translator_libretranslate_debug_SRCS}
    TEST_NAME libretranslateengineclienttest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n KF5::ConfigCore
)

######
ecm_add_test(libretranslateengineconfigurewidgettest.cpp libretranslateengineconfigurewidgettest.h
    ../libretranslateengineconfigurewidget.cpp
    TEST_NAME libretranslateengineconfigurewidgettest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n
)

######
ecm_add_test(libretranslateengineconfiguredialogtest.cpp libretranslateengineconfiguredialogtest.h
    ../libretranslateengineconfiguredialog.cpp ../libretranslateengineconfigurewidget.cpp
    TEST_NAME libretranslateengineconfiguredialogtest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n
)

######
ecm_add_test(libretranslateengineutiltest.cpp libretranslateengineutiltest.h
    ../libretranslateengineutil.cpp
    TEST_NAME libretranslateengineutiltest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n
)
