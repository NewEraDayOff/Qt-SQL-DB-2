QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dbcheck.cpp \
    dbitembuying.cpp \
    dbplay.cpp \
    dbwork.cpp \
    login_ui/dbadmin.cpp \
    login_ui/dblogin.cpp \
    login_ui/dbcreation.cpp \
    login_ui/dbretrieve.cpp \
    main.cpp \
    mainwindow.cpp \
    task/dbrequest.cpp \
    task/dbtask.cpp \
    work/dbachievement.cpp \
    work/dbdeveloper.cpp \
    work/dbgame.cpp \
    work/dbitem.cpp \
    work/dbpublisher.cpp \
    work/dbuser.cpp

HEADERS += \
    dbcheck.h \
    dbitembuying.h \
    dbplay.h \
    dbwork.h \
    login_ui/dbadmin.h \
    login_ui/dblogin.h \
    login_ui/dbcreation.h \
    login_ui/dbretrieve.h \
    mainwindow.h \
    task/dbrequest.h \
    task/dbstruct.h \
    task/dbtask.h \
    work/dbachievement.h \
    work/dbdeveloper.h \
    work/dbgame.h \
    work/dbitem.h \
    work/dbpublisher.h \
    work/dbuser.h

FORMS += \
    dbitembuying.ui \
    dbplay.ui \
    login_ui/dbadmin.ui \
    login_ui/dblogin.ui \
    login_ui/dbcreation.ui \
    login_ui/dbretrieve.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Res.qrc \
    breeze.qrc
