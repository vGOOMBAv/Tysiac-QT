QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    card.cpp \
    carddeck.cpp \
    game3players.cpp \
    gameiterationscore.cpp \
    main.cpp \
    mainwindow.cpp \
    player.cpp

HEADERS += \
    card.h \
    carddeck.h \
    game3players.h \
    gameiterationscore.h \
    mainwindow.h \
    player.h

FORMS += \
    game3players.ui \
    mainwindow.ui

TRANSLATIONS += \
    Tysiac_pl_PL.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
