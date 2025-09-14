/****************************************************************************
** Meta object code from reading C++ file 'gamewindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../gamewindow.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gamewindow.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10GameWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto GameWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10GameWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GameWindow",
        "onTimerTimeout",
        "",
        "onHintTimerTimeout",
        "onDizzyTimerTimeout",
        "Player&",
        "player",
        "QTimer*",
        "timer",
        "onItemSpawnTimerTimeout",
        "updateWindowTitle",
        "status",
        "initializeAudioSystem",
        "initializeZombieAnimation",
        "initializeZombie2Animation",
        "triggerZombieAttackAnimation",
        "triggerZombie2AttackAnimation",
        "loadMenuBackground",
        "loadGameBackground",
        "loadSaveBackground",
        "loadBrainPropImage",
        "adjustWindowSizeToBackground",
        "playEliminationSound",
        "playItemSound",
        "playSelectSound",
        "playWinSound",
        "playBackgroundMusic",
        "stopBackgroundMusic",
        "playGameMusic",
        "stopGameMusic",
        "initializeCompactMenu",
        "createGameMenu",
        "createHelpMenu",
        "createSaveMenu",
        "createSettingsMenu"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'onTimerTimeout'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onHintTimerTimeout'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDizzyTimerTimeout'
        QtMocHelpers::SlotData<void(Player &, QTimer *)>(4, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 5, 6 }, { 0x80000000 | 7, 8 },
        }}),
        // Slot 'onItemSpawnTimerTimeout'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateWindowTitle'
        QtMocHelpers::SlotData<void(const QString &)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 11 },
        }}),
        // Slot 'updateWindowTitle'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate | QMC::MethodCloned, QMetaType::Void),
        // Slot 'initializeAudioSystem'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'initializeZombieAnimation'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'initializeZombie2Animation'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'triggerZombieAttackAnimation'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'triggerZombie2AttackAnimation'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'loadMenuBackground'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'loadGameBackground'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'loadSaveBackground'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'loadBrainPropImage'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'adjustWindowSizeToBackground'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'playEliminationSound'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'playItemSound'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'playSelectSound'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'playWinSound'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'playBackgroundMusic'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'stopBackgroundMusic'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'playGameMusic'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'stopGameMusic'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'initializeCompactMenu'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'createGameMenu'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'createHelpMenu'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'createSaveMenu'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'createSettingsMenu'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GameWindow, qt_meta_tag_ZN10GameWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GameWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10GameWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10GameWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10GameWindowE_t>.metaTypes,
    nullptr
} };

void GameWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GameWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->onTimerTimeout(); break;
        case 1: _t->onHintTimerTimeout(); break;
        case 2: _t->onDizzyTimerTimeout((*reinterpret_cast< std::add_pointer_t<Player&>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QTimer*>>(_a[2]))); break;
        case 3: _t->onItemSpawnTimerTimeout(); break;
        case 4: _t->updateWindowTitle((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->updateWindowTitle(); break;
        case 6: _t->initializeAudioSystem(); break;
        case 7: _t->initializeZombieAnimation(); break;
        case 8: _t->initializeZombie2Animation(); break;
        case 9: _t->triggerZombieAttackAnimation(); break;
        case 10: _t->triggerZombie2AttackAnimation(); break;
        case 11: _t->loadMenuBackground(); break;
        case 12: _t->loadGameBackground(); break;
        case 13: _t->loadSaveBackground(); break;
        case 14: _t->loadBrainPropImage(); break;
        case 15: _t->adjustWindowSizeToBackground(); break;
        case 16: _t->playEliminationSound(); break;
        case 17: _t->playItemSound(); break;
        case 18: _t->playSelectSound(); break;
        case 19: _t->playWinSound(); break;
        case 20: _t->playBackgroundMusic(); break;
        case 21: _t->stopBackgroundMusic(); break;
        case 22: _t->playGameMusic(); break;
        case 23: _t->stopGameMusic(); break;
        case 24: _t->initializeCompactMenu(); break;
        case 25: _t->createGameMenu(); break;
        case 26: _t->createHelpMenu(); break;
        case 27: _t->createSaveMenu(); break;
        case 28: _t->createSettingsMenu(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QTimer* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *GameWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GameWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10GameWindowE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int GameWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 29)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 29;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 29)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 29;
    }
    return _id;
}
QT_WARNING_POP
