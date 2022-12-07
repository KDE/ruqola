ecm_add_test(lingvaengineclienttest.cpp lingvaengineclienttest.h ../lingvaengineclient.cpp ../lingvaengineplugin.cpp
    TEST_NAME lingvaengineclienttest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n KF5::ConfigCore
)
