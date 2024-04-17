QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts sql

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    account.cpp \
    addfinance.cpp \
    dashboard.cpp \
    main.cpp \
    quitdialog.cpp \
    signup.cpp \
    startwindow.cpp \
    task.cpp

HEADERS += \
    account.h \
    addfinance.h \
    dashboard.h \
    quitdialog.h \
    signup.h \
    startwindow.h \
    task.h

FORMS += \
    account.ui \
    addfinance.ui \
    dashboard.ui \
    quitdialog.ui \
    signup.ui \
    startwindow.ui \
    task.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc

DISTFILES += \
    hmdb.db
