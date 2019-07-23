TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    Exeption.h \
    Vector3D.h \
    Matrix3D/SysLinearEquat3D.h \
    Segment3D.h \
    Matrix3D/Matrix3D.h \
    Matrix4x4/SysLinearEquat3D.h \
    NumComp.h \
    Matrix4x4/Matrix4x4.h
