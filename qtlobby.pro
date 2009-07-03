# #####################################################################
# Automatically generated by qmake (2.01a) Wed Mar 5 00:00:30 2008
# #####################################################################
TEMPLATE = app
TARGET = qtlobby
OBJECTS_DIR = debug
MOC_DIR = debug
OBJMOC = debug
DESTDIR = debug
DESTDIR_TARGET = qtlobby
SRCMOC = debug
QMAKE_CLEAN = debug/*
DEPENDPATH += .
INCLUDEPATH += .
RESOURCES = resources.qrc
UI_HEADERS_DIR = src
DEFINES += 'SVN_REV=\\"$(shell svnversion -n .)\\"'
DEFINES += RPM_OPT_FLAGS

target.path += "/usr/bin"
INSTALLS += target

# SVNDEF := -DSVN_REV=444
# DEFINES += SVN_REV=444
# QMAKE_CFLAGS += $(SVNDEF)
# QMAKE_CXXFLAGS += $(SVNDEF)
include(src/modeltest/modeltest.pri)

# Input
HEADERS += src/MainWindow.h \
    src/ServerContextState.h \
    src/StatusTracker.h \
    src/NetworkInterface.h \
    src/Command.h \
    src/AbstractStateClient.h \
    src/CommandAssigner.h \
    src/ConnectionWidget.h \
    src/Channel.h \
    src/InfoChannel.h \
    src/LobbyTabs.h \
    src/AbstractLobbyTab.h \
    src/User.h \
    src/Users.h \
    src/Settings.h \
    src/AgreementWidget.h \
    src/InputLine.h \
    src/Battle.h \
    src/PrivateChannel.h \
    src/AbstractChannel.h \
    src/UserTreeModel.h \
    src/UserManager.h \
    src/BattleChannel.h \
    src/BattleManager.h \
    src/Battles.h \
    src/BattleTreeModel.h \
    src/ServerConfig.h \
    src/TLDList.h \
    src/UnitSyncLib.h \
    src/AudioBackend.h \
    src/Preference.h \
    src/GamePasswordWidget.h \
    src/MapSelector.h \
    src/BattleUserTreeModel.h \
    src/TreeSortFilterProxyModel.h \
    src/mapElementWidget.h \
    src/ConfigElement.h \
    src/Singleton.h \
    src/ModOption.h \
    src/StylesheetDialog.h \
    src/ChannelTextBrowser.h \
    src/MapWidget.h \
    src/RawHeightMap.h \
    src/MapInfoLoader.h \
    src/CompletionListWidget.h \
    src/MapOverviewDialog.h \
    src/MapRendererWidget.h \
    src/GLProgressDialog.h
SOURCES += src/main.cpp \
    src/MainWindow.cpp \
    src/ServerContextState.cpp \
    src/StatusTracker.cpp \
    src/NetworkInterface.cpp \
    src/Command.cpp \
    src/AbstractStateClient.cpp \
    src/CommandAssigner.cpp \
    src/ConnectionWidget.cpp \
    src/Channel.cpp \
    src/InfoChannel.cpp \
    src/LobbyTabs.cpp \
    src/AbstractLobbyTab.cpp \
    src/User.cpp \
    src/Users.cpp \
    src/Settings.cpp \
    src/AgreementWidget.cpp \
    src/InputLine.cpp \
    src/Battle.cpp \
    src/UserTreeModel.cpp \
    src/PrivateChannel.cpp \
    src/AbstractChannel.cpp \
    src/UserManager.cpp \
    src/BattleChannel.cpp \
    src/BattleManager.cpp \
    src/Battles.cpp \
    src/BattleTreeModel.cpp \
    src/ServerConfig.cpp \
    src/TLDList.cpp \
    src/UnitSyncLib.cpp \
    src/AudioBackend.cpp \
    src/Preference.cpp \
    src/GamePasswordWidget.cpp \
    src/MapSelector.cpp \
    src/BattleUserTreeModel.cpp \
    src/TreeSortFilterProxyModel.cpp \
    src/mapElementWidget.cpp \
    src/ConfigElement.cpp \
    src/Singleton.cpp \
    src/ModOption.cpp \
    src/StylesheetDialog.cpp \
    src/ChannelTextBrowser.cpp \
    src/MapWidget.cpp \
    src/RawHeightMap.cpp \
    src/MapInfoLoader.cpp \
    src/CompletionListWidget.cpp \
    src/MapOverviewDialog.cpp \
    src/MapRendererWidget.cpp \
    src/GLProgressDialog.cpp
win32 { 
    CONFIG += release
    CONFIG -= debug
}
unix { 
    CONFIG -= release
    CONFIG += debug
}
QT += gui \
    network \
    opengl
FORMS += ui/mainWidget.ui \
    ui/connectionWidget.ui \
    ui/mapSelectorWidget.ui \
    ui/battleWidget.ui \
    ui/gamePasswordWidget.ui \
    ui/aboutWidget.ui \
    ui/agreementWidget.ui \
    ui/abstractChannelWidget.ui \
    ui/preferenceWidget.ui \
    ui/configElement.ui \
    ui/StylesheetDialog.ui \
    ui/MapOverviewDialog.ui \
    ui/GLProgressDialog.ui
DISTFILES += doc/ProtocolDescription.xml \
    TODO \
    doc/xml2html.xsl \
    doc/z
