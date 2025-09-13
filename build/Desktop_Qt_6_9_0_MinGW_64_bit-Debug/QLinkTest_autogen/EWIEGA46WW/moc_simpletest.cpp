/****************************************************************************
** Meta object code from reading C++ file 'simpletest.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../simpletest.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simpletest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10SimpleTestE_t {};
} // unnamed namespace

template <> constexpr inline auto SimpleTest::qt_create_metaobjectdata<qt_meta_tag_ZN10SimpleTestE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SimpleTest",
        "initTestCase",
        "",
        "cleanupTestCase",
        "testCanLineConnection_Horizontal",
        "testCanLineConnection_Vertical",
        "testCanLineConnection_EdgeCases",
        "testHasOneCornerPath_Valid",
        "testHasOneCornerPath_Invalid",
        "testHasOneCornerPath_EdgeCases",
        "testHasTwoCornersPath_Valid",
        "testHasTwoCornersPath_Invalid",
        "testHasTwoCornersPath_EdgeCases",
        "testCanEliminate_NoPath",
        "testCanEliminate_SamePosition",
        "testCanEliminate_DifferentTypes",
        "testCanEliminate_EmptyBlocks",
        "testCanEliminate_OutOfBounds",
        "testBoundaryConditions",
        "testCornerPositions",
        "testEdgePositions",
        "testInvalidInputs",
        "testExtremeCases"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'initTestCase'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'cleanupTestCase'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testCanLineConnection_Horizontal'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testCanLineConnection_Vertical'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testCanLineConnection_EdgeCases'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testHasOneCornerPath_Valid'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testHasOneCornerPath_Invalid'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testHasOneCornerPath_EdgeCases'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testHasTwoCornersPath_Valid'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testHasTwoCornersPath_Invalid'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testHasTwoCornersPath_EdgeCases'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testCanEliminate_NoPath'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testCanEliminate_SamePosition'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testCanEliminate_DifferentTypes'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testCanEliminate_EmptyBlocks'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testCanEliminate_OutOfBounds'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testBoundaryConditions'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testCornerPositions'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testEdgePositions'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testInvalidInputs'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testExtremeCases'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SimpleTest, qt_meta_tag_ZN10SimpleTestE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SimpleTest::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10SimpleTestE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10SimpleTestE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10SimpleTestE_t>.metaTypes,
    nullptr
} };

void SimpleTest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SimpleTest *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->initTestCase(); break;
        case 1: _t->cleanupTestCase(); break;
        case 2: _t->testCanLineConnection_Horizontal(); break;
        case 3: _t->testCanLineConnection_Vertical(); break;
        case 4: _t->testCanLineConnection_EdgeCases(); break;
        case 5: _t->testHasOneCornerPath_Valid(); break;
        case 6: _t->testHasOneCornerPath_Invalid(); break;
        case 7: _t->testHasOneCornerPath_EdgeCases(); break;
        case 8: _t->testHasTwoCornersPath_Valid(); break;
        case 9: _t->testHasTwoCornersPath_Invalid(); break;
        case 10: _t->testHasTwoCornersPath_EdgeCases(); break;
        case 11: _t->testCanEliminate_NoPath(); break;
        case 12: _t->testCanEliminate_SamePosition(); break;
        case 13: _t->testCanEliminate_DifferentTypes(); break;
        case 14: _t->testCanEliminate_EmptyBlocks(); break;
        case 15: _t->testCanEliminate_OutOfBounds(); break;
        case 16: _t->testBoundaryConditions(); break;
        case 17: _t->testCornerPositions(); break;
        case 18: _t->testEdgePositions(); break;
        case 19: _t->testInvalidInputs(); break;
        case 20: _t->testExtremeCases(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *SimpleTest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SimpleTest::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10SimpleTestE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SimpleTest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 21;
    }
    return _id;
}
QT_WARNING_POP
