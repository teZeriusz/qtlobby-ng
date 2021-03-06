include(${QT_USE_FILE})
include_directories(${QTSCRIPTTOOLS_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
CONFIGURE_FILE( ${CMAKE_SOURCE_DIR}/src/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )
CONFIGURE_FILE( ${CMAKE_SOURCE_DIR}/src/qtlobby.rc.cmake ${CMAKE_CURRENT_BINARY_DIR}/qtlobby.rc )

# source files
set(qtlobby_SRCS
    #sources
AbstractChannel.cpp
AbstractLobbyTab.cpp
AbstractStateClient.cpp
AgreementWidget.cpp    
AudioBackend.cpp       
BattleChannel.cpp      
BattleCloseFirstWidget.cpp
Battle.cpp                
BattleHost.cpp            
BattleHostingDialog.cpp   
BattleManager.cpp         
BattlePasswordWidget.cpp  
Battles.cpp               
BattleTreeModel.cpp       
BattleUserTreeModel.cpp   
Channel.cpp               
ChannelTextBrowser.cpp    
CommandAssigner.cpp       
Command.cpp               
CompletionListWidget.cpp  
ConnectionWidget.cpp      
Downloader.cpp                   
DownloadsDialog.cpp              
DownloadsModel.cpp               
glextensions.cpp                 
GLHeightMap.cpp                  
GLMaterial.cpp                   
GLObject.cpp                     
GLProgressDialog.cpp             
GLWaterPlane.cpp                 
InfoChannel.cpp                  
InputLine.cpp                    
LobbyTabs.cpp                               
MainWindow.cpp                   
MapElementWidget.cpp             
MapInfoLoader.cpp                
MapOverviewDialog.cpp            
MapRendererWidget.cpp            
MapSelector.cpp                  
MapWidget.cpp                    
ModOption.cpp                    
NetworkInterface.cpp             
Notification.cpp                 
PathManager.cpp                  
Preference.cpp                   
PreferencePathElement.cpp        
PrivateChannel.cpp               
ProgressBarItemDelegate.cpp      
RawHeightMap.cpp
ScriptingDialog.cpp
ServerConfig.cpp
ServerContextState.cpp
ServerProfilesModel.cpp
Settings.cpp
ShaderSet.cpp
Singleton.cpp
StatusTracker.cpp
StylesheetDialog.cpp
TLDList.cpp
ToolBarWidget.cpp
TrayIconNotificationBackend.cpp
TreeSortFilterProxyModel.cpp
UnitSyncLib.cpp
User.cpp
UserGroup.cpp
UserGroupsDialog.cpp
UserManager.cpp
UserMenuBuilder.cpp
Users.cpp
UsersTreeView.cpp
UserTreeModel.cpp
sqadsprototypes.cpp
History.cpp
HistoryDialog.cpp
)

if(MINGW)
if(BUILDBOT)
SET(WINDRES_BINARY mingw32-windres)
else(BUILDBOT)
SET(WINDRES_BINARY windres.exe)
endif(BUILDBOT)
# resource compilation for mingw
ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/qtlobby_rc.o
COMMAND ${WINDRES_BINARY} -I${CMAKE_CURRENT_SOURCE_DIR}
-i ${CMAKE_CURRENT_BINARY_DIR}/qtlobby.rc
-o ${CMAKE_CURRENT_BINARY_DIR}/qtlobby_rc.o)
SET(qtlobby_SRCS ${qtlobby_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/qtlobby_rc.o)
else(MINGW)
SET(qtlobby_SRCS ${qtlobby_SRCS} qtlobby.rc)
endif(MINGW)

set(qtlobby_SRCS_H 
AbstractChannel.h
AbstractLobbyTab.h
AbstractNotificationBackend.h
AbstractStateClient.h        
AgreementWidget.h            
AudioBackend.h               
BattleChannel.h              
BattleCloseFirstWidget.h     
Battle.h                     
BattleHost.h                 
BattleHostingDialog.h        
BattleManager.h              
BattlePasswordWidget.h       
Battles.h                    
BattleTreeModel.h            
BattleUserTreeModel.h        
Channel.h                    
ChannelTextBrowser.h         
CommandAssigner.h            
Command.h                    
CompletionListWidget.h           
ConnectionWidget.h           
Downloader.h                   
DownloadsDialog.h              
DownloadsModel.h               
glextensions.h                 
GLHeightMap.h                  
GLMaterial.h                   
GLObject.h                     
GLProgressDialog.h             
GLWaterPlane.h                 
InfoChannel.h                  
InputLine.h                    
LobbyTabs.h                    
MainWindow.h                   
MapElementWidget.h             
MapInfoLoader.h                
MapOverviewDialog.h            
MapRendererWidget.h            
MapSelector.h                  
MapWidget.h                    
ModOption.h                    
NetworkInterface.h             
Notification.h                 
PathManager.h                  
Preference.h                   
PreferencePathElement.h        
PrivateChannel.h               
ProgressBarItemDelegate.h      
RawHeightMap.h                 
ScriptingDialog.h              
ServerConfig.h                 
ServerContextState.h           
ServerProfilesModel.h          
Settings.h                     
ShaderSet.h                    
Singleton.h                    
StatusTracker.h                
StylesheetDialog.h             
TLDList.h                      
ToolBarWidget.h                
TrayIconNotificationBackend.h  
TreeSortFilterProxyModel.h
UnitSyncLib.h
UserGroup.h
UserGroupsDialog.h
User.h
UserManager.h
UserMenuBuilder.h
Users.h
UsersTreeView.h
UserTreeModel.h
sqadsprototypes.h
History.h
HistoryDialog.h
)     

if(UNIX)
set(qtlobby_SRCS ${qtlobby_SRCS} DBusVisualNotificationBackend.cpp)
set(qtlobby_SRCS_H ${qtlobby_SRCS_H} DBusVisualNotificationBackend.h)
endif(UNIX)

QT4_WRAP_CPP(qtlobby_SRCS_MOC ${qtlobby_SRCS_H})
      
# UI files
set( qtlobby_UIS
../ui/AboutWidget.ui
../ui/AgreementWidget.ui
../ui/BattleCloseFirstWidget.ui
../ui/BattleHostingDialog.ui
../ui/BattlePasswordWidget.ui
../ui/BattleWindowForm.ui
../ui/ConnectionWidget.ui
../ui/DownloadsDialog.ui
../ui/GLProgressDialog.ui
../ui/LoginDialog.ui
../ui/MainWidget.ui
../ui/MapOverviewDialog.ui
../ui/MapSelectorWidget.ui
../ui/PreferencePathElement.ui
../ui/PreferenceWidget.ui
../ui/ScriptingDialog.ui
../ui/StylesheetDialog.ui
../ui/ToolBarWidget.ui
../ui/UserGroupsDialog.ui
../ui/HistoryDialog.ui
)     
QT4_WRAP_UI(qtlobby_UIS_H ${qtlobby_UIS})
 
      
#QT4_ADD_RESOURCES(qtlobby_RES ../resources.qrc)
if(TRANSLATIONS)
  QT4_ADD_TRANSLATION(qtlobby_TRANS
  ../i18n/qtlobby_de.ts
  ../i18n/qtlobby_es.ts
  ../i18n/qtlobby_fi.ts
  ../i18n/qtlobby_ru.ts
)
endif(TRANSLATIONS)
      
# linking compiling
add_executable(qtlobby ${qtlobby_SRCS} ${qtlobby_SRCS_HH} ${qtlobby_UIS_H} ${qtlobby_SRCS_MOC} ${qtlobby_RES} ${qtlobby_TRANS} main.cpp)

if(BUILDBOT)
SET_TARGET_PROPERTIES(qtlobby PROPERTIES LINK_FLAGS "-static-libgcc -Wl,-subsystem,windows")
endif(BUILDBOT)

target_link_libraries(qtlobby ${QT_LIBRARIES} ${QSCINTILLA_LIBRARIES} ${QTSCRIPTTOOLS_LIBRARIES} ${OPENGL_LIBRARIES})

#install
install(TARGETS qtlobby DESTINATION bin)

