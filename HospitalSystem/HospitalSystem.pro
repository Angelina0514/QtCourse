QT += core gui widgets sql

TARGET = HospitalSystem
TEMPLATE = app

CONFIG += c++17

SOURCES += \
    main.cpp \
    dbmanager.cpp \
    logindialog.cpp \
    patientdialog.cpp \
    mainwindow.cpp

HEADERS += \
    dbmanager.h \
    logindialog.h \
    patientdialog.h \
    mainwindow.h
