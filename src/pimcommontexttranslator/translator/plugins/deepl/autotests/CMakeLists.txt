ecm_add_test(deeplengineclienttest.cpp deeplengineclienttest.h ../deeplengineclient.cpp ../deeplengineplugin.cpp
    TEST_NAME deeplengineclienttest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n
)
