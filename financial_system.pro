QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

VPATH += ./modules

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    database/database.cpp \
    database/userdb.cpp \
    modules/entities/bank/accountmanager.cpp \
    modules/entities/bank/creditmanager.cpp \
    modules/view/addcardwidget.cpp \
    modules/view/clientwindow.cpp \
    modules/view/creditwidget.cpp \
    modules/view/historywidget.cpp \
    modules/view/systemwindow.cpp \
    modules/view/transferwidget.cpp \
    view/authwidget.cpp \

HEADERS += \
    mainwindow.h \
    modules/database/databasemanager.h \
    modules/database/idgenerator.h \
    modules/entities/ISystemObject.h \
    modules/entities/IUser.h \
    modules/entities/bank/IHistoryManager.h \
    modules/entities/bank/accountmanager.h \
    modules/entities/bank/bank.h \
    modules/entities/bank/bankaccount.h \
    modules/entities/bank/credit.h \
    modules/entities/bank/creditmanager.h \
    modules/entities/bank/installment.h \
    modules/entities/bank/loan.h \
    modules/entities/bank/request.h \
    modules/entities/bank/transaction.h \
    modules/entities/entity.h \
    modules/entities/individual.h \
    modules/view/addcardwidget.h \
    modules/view/clientwindow.h \
    modules/view/creditwidget.h \
    modules/view/historywidget.h \
    modules/view/managerfactory.h \
    modules/view/systemwindow.h \
    modules/view/transferwidget.h \
    singleton.h modes.h \
    database/database.h \
    database/userdb.h \
    entities/systemuser.h \
    view/authwidget.h \
    entities/individual.h

FORMS += \
    mainwindow.ui \
    modules/view/addcardwidget.ui \
    modules/view/clientwindow.ui \
    modules/view/creditwidget.ui \
    modules/view/historywidget.ui \
    modules/view/systemwindow.ui \
    modules/view/transferwidget.ui \
    view/authwidget.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
