// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "UnitSyncLib.h"
#include <QColor>
#include <QMutex>
#include <QMutexLocker>

QMutex unitsync_mutex;

//for locking debugging:
//#define NON_REENTRANT qDebug() << "Locking at" << __LINE__; QMutexLocker no_reentrance_mutex_locker(&unitsync_mutex); qDebug() << "Locked";
//Normal
#define NON_REENTRANT QMutexLocker no_reentrance_mutex_locker(&unitsync_mutex);
#define MANUAL_LOCK unitsync_mutex.lock();
#define MANUAL_UNLOCK unitsync_mutex.unlock();

UnitSyncLib::UnitSyncLib( QObject *parent ) : QObject( parent ) {
    unitsynclib = new QLibrary( this );
    settings = Settings::Instance();
    library_loaded = false;

    if (!unitsynclib->isLoaded()) { // maybe this was useless after all (aj)
        if ( loadLibrary() ) {
            //     qDebug() << "library is usable";
            TestCall();
            //     QMessageBox::information( NULL, "unitSyncLib", "library loaded");
        } else {
            qDebug() << tr("UnitSyncLibrary is not usable!");
        }
    }

    m_gameOptionKeys << "diminishingmms"
            << "disablemapdamage"
            << "fixedallies"
            << "ghostedbuildings"
            << "limitdgun"
            << "deathmode"
            << "gamemode"
            << "startpostype";
}

bool UnitSyncLib::loadLibrary() {
    NON_REENTRANT;
    QString lib_with_path = settings->value("unitsync").toString();
    QFileInfo fi( lib_with_path );

#ifdef Q_WS_WIN
    /*  Change current dir cause otherwise windows cant load unitsync (aj) */
    QString originaldir = QDir::currentPath();
    QDir::setCurrent(fi.absolutePath());
#endif

    /* the next line is very important to load/find the shared libraries used by
     the shared library we are using, namely unitsync.dll (js)*/
    QApplication::instance()->addLibraryPath(fi.absolutePath());
    QApplication::instance()->addLibraryPath(".");

    //   QMessageBox::information( NULL, "unitSyncLib", QString("%1")
    //       .arg(fi.absolutePath()));

    if (!QFile::exists(lib_with_path)) {
        QMessageBox::information( NULL, tr("unitSyncLib - library not found"),
                                  tr("Unitsync library was not found or is unusable."));
        return false;
    }
    unitsynclib->setFileName( lib_with_path );
    //   unitsynclib->setLoadHints(QLibrary::ExportExternalSymbolsHint|QLibrary::LoadArchiveMemberHint);
    unitsynclib->load();

    if ( !unitsynclib->isLoaded() ) {
        QMessageBox::information( NULL, tr("unitSyncLib - library not loaded"),
                                  QString( "%1 -- %2" )
                                  .arg( unitsynclib->errorString() )
                                  .arg( lib_with_path ) );
        return false;
    }

    m_GetSpringVersion  = ( GetSpringVersion ) unitsynclib->resolve( "GetSpringVersion" );
    m_Message  = ( Message ) unitsynclib->resolve( "Message" );
    m_UnInit  = ( UnInit ) unitsynclib->resolve( "UnInit" );
    m_Init  = ( Init ) unitsynclib->resolve( "Init" );
    m_InitArchiveScanner  = ( InitArchiveScanner ) unitsynclib->resolve( "InitArchiveScanner" );
    m_ProcessUnits  = ( ProcessUnits ) unitsynclib->resolve( "ProcessUnits" );
    m_ProcessUnitsNoChecksum  = ( ProcessUnitsNoChecksum ) unitsynclib->resolve( "ProcessUnitsNoChecksum" );
    m_GetCurrentList  = ( GetCurrentList ) unitsynclib->resolve( "GetCurrentList" );
    m_AddClient  = ( AddClient ) unitsynclib->resolve( "AddClient" );
    m_RemoveClient  = ( RemoveClient ) unitsynclib->resolve( "RemoveClient" );
    m_GetClientDiff  = ( GetClientDiff ) unitsynclib->resolve( "GetClientDiff" );
    m_InstallClientDiff  = ( InstallClientDiff ) unitsynclib->resolve( "InstallClientDiff" );
    m_GetUnitCount  = ( GetUnitCount ) unitsynclib->resolve( "GetUnitCount" );
    m_GetUnitName  = ( GetUnitName ) unitsynclib->resolve( "GetUnitName" );
    m_GetFullUnitName  = ( GetFullUnitName ) unitsynclib->resolve( "GetFullUnitName" );
    m_IsUnitDisabled  = ( IsUnitDisabled ) unitsynclib->resolve( "IsUnitDisabled" );
    m_IsUnitDisabledByClient  = ( IsUnitDisabledByClient ) unitsynclib->resolve( "IsUnitDisabledByClient" );
    m_AddArchive  = ( AddArchive ) unitsynclib->resolve( "AddArchive" );
    m_AddAllArchives  = ( AddAllArchives ) unitsynclib->resolve( "AddAllArchives" );
    m_RemoveAllArchives  = ( RemoveAllArchives ) unitsynclib->resolve( "RemoveAllArchives" );
    m_GetArchiveChecksum  = ( GetArchiveChecksum ) unitsynclib->resolve( "GetArchiveChecksum" );
    m_GetArchivePath  = ( GetArchivePath ) unitsynclib->resolve( "GetArchivePath" );
    m_GetMapCount  = ( GetMapCount ) unitsynclib->resolve( "GetMapCount" );
    m_GetMapName  = ( GetMapName ) unitsynclib->resolve( "GetMapName" );
    m_GetMapInfoEx  = ( GetMapInfoEx ) unitsynclib->resolve( "GetMapInfoEx" );
    m_GetMapInfo  = ( GetMapInfo ) unitsynclib->resolve( "GetMapInfo" );
    m_GetMapArchiveCount  = ( GetMapArchiveCount ) unitsynclib->resolve( "GetMapArchiveCount" );
    m_GetMapArchiveName  = ( GetMapArchiveName ) unitsynclib->resolve( "GetMapArchiveName" );
    m_GetMapChecksum  = ( GetMapChecksum ) unitsynclib->resolve( "GetMapChecksum" );
    m_GetMapChecksumFromName  = ( GetMapChecksumFromName ) unitsynclib->resolve( "GetMapChecksumFromName" );
    m_GetMinimap  = ( GetMinimap ) unitsynclib->resolve( "GetMinimap" );
    m_GetInfoMapSize = ( GetInfoMapSize ) unitsynclib->resolve( "GetInfoMapSize" );
    m_GetInfoMap = ( GetInfoMap ) unitsynclib->resolve( "GetInfoMap" );
    m_GetPrimaryModCount  = ( GetPrimaryModCount ) unitsynclib->resolve( "GetPrimaryModCount" );
    m_GetPrimaryModName  = ( GetPrimaryModName ) unitsynclib->resolve( "GetPrimaryModName" );
    m_GetPrimaryModShortName  = ( GetPrimaryModShortName ) unitsynclib->resolve( "GetPrimaryModShortName" );
    m_GetPrimaryModVersion  = ( GetPrimaryModVersion ) unitsynclib->resolve( "GetPrimaryModVersion" );
    m_GetPrimaryModMutator  = ( GetPrimaryModMutator ) unitsynclib->resolve( "GetPrimaryModMutator" );
    m_GetPrimaryModGame  = ( GetPrimaryModGame ) unitsynclib->resolve( "GetPrimaryModGame" );
    m_GetPrimaryModShortGame  = ( GetPrimaryModShortGame ) unitsynclib->resolve( "GetPrimaryModShortGame" );
    m_GetPrimaryModDescription  = ( GetPrimaryModDescription ) unitsynclib->resolve( "GetPrimaryModDescription" );
    m_GetPrimaryModArchive  = ( GetPrimaryModArchive ) unitsynclib->resolve( "GetPrimaryModArchive" );
    m_GetPrimaryModArchiveCount  = ( GetPrimaryModArchiveCount ) unitsynclib->resolve( "GetPrimaryModArchiveCount" );
    m_GetPrimaryModArchiveList  = ( GetPrimaryModArchiveList ) unitsynclib->resolve( "GetPrimaryModArchiveList" );
    m_GetPrimaryModIndex  = ( GetPrimaryModIndex ) unitsynclib->resolve( "GetPrimaryModIndex" );
    m_GetPrimaryModChecksum  = ( GetPrimaryModChecksum ) unitsynclib->resolve( "GetPrimaryModChecksum" );
    m_GetPrimaryModChecksumFromName  = ( GetPrimaryModChecksumFromName ) unitsynclib->resolve( "GetPrimaryModChecksumFromName" );
    m_GetSideCount  = ( GetSideCount ) unitsynclib->resolve( "GetSideCount" );
    m_GetSideName  = ( GetSideName ) unitsynclib->resolve( "GetSideName" );
    m_GetLuaAICount  = ( GetLuaAICount ) unitsynclib->resolve( "GetLuaAICount" );
    m_GetLuaAIName  = ( GetLuaAIName ) unitsynclib->resolve( "GetLuaAIName" );
    m_GetLuaAIDesc  = ( GetLuaAIDesc ) unitsynclib->resolve( "GetLuaAIDesc" );
    m_GetMapOptionCount  = ( GetMapOptionCount ) unitsynclib->resolve( "GetMapOptionCount" );
    m_GetModOptionCount  = ( GetModOptionCount ) unitsynclib->resolve( "GetModOptionCount" );
    m_GetOptionKey  = ( GetOptionKey ) unitsynclib->resolve( "GetOptionKey" );
    m_GetOptionName  = ( GetOptionName ) unitsynclib->resolve( "GetOptionName" );
    m_GetOptionDesc  = ( GetOptionDesc ) unitsynclib->resolve( "GetOptionDesc" );
    m_GetOptionType  = ( GetOptionType ) unitsynclib->resolve( "GetOptionType" );
    m_GetOptionBoolDef  = ( GetOptionBoolDef ) unitsynclib->resolve( "GetOptionBoolDef" );
    m_GetOptionNumberDef  = ( GetOptionNumberDef ) unitsynclib->resolve( "GetOptionNumberDef" );
    m_GetOptionNumberMin  = ( GetOptionNumberMin ) unitsynclib->resolve( "GetOptionNumberMin" );
    m_GetOptionNumberMax  = ( GetOptionNumberMax ) unitsynclib->resolve( "GetOptionNumberMax" );
    m_GetOptionNumberStep  = ( GetOptionNumberStep ) unitsynclib->resolve( "GetOptionNumberStep" );
    m_GetOptionSection = ( GetOptionSection ) unitsynclib->resolve( "GetOptionSection" );
    m_GetOptionStringDef  = ( GetOptionStringDef ) unitsynclib->resolve( "GetOptionStringDef" );
    m_GetOptionStringMaxLen  = ( GetOptionStringMaxLen ) unitsynclib->resolve( "GetOptionStringMaxLen" );
    m_GetOptionListCount  = ( GetOptionListCount ) unitsynclib->resolve( "GetOptionListCount" );
    m_GetOptionListDef  = ( GetOptionListDef ) unitsynclib->resolve( "GetOptionListDef" );
    m_GetOptionListItemKey  = ( GetOptionListItemKey ) unitsynclib->resolve( "GetOptionListItemKey" );
    m_GetOptionListItemName  = ( GetOptionListItemName ) unitsynclib->resolve( "GetOptionListItemName" );
    m_GetOptionListItemDesc  = ( GetOptionListItemDesc ) unitsynclib->resolve( "GetOptionListItemDesc" );
    m_GetModValidMapCount  = ( GetModValidMapCount ) unitsynclib->resolve( "GetModValidMapCount" );
    m_GetModValidMap  = ( GetModValidMap ) unitsynclib->resolve( "GetModValidMap" );
    m_OpenFileVFS  = ( OpenFileVFS ) unitsynclib->resolve( "OpenFileVFS" );
    m_CloseFileVFS  = ( CloseFileVFS ) unitsynclib->resolve( "CloseFileVFS" );
    m_ReadFileVFS  = ( ReadFileVFS ) unitsynclib->resolve( "ReadFileVFS" );
    m_FileSizeVFS  = ( FileSizeVFS ) unitsynclib->resolve( "FileSizeVFS" );
    m_InitFindVFS  = ( InitFindVFS ) unitsynclib->resolve( "InitFindVFS" );
    m_FindFilesVFS  = ( FindFilesVFS ) unitsynclib->resolve( "FindFilesVFS" );
    m_OpenArchive  = ( OpenArchive ) unitsynclib->resolve( "OpenArchive" );
    m_CloseArchive  = ( CloseArchive ) unitsynclib->resolve( "CloseArchive" );
    m_FindFilesArchive  = ( FindFilesArchive ) unitsynclib->resolve( "FindFilesArchive" );
    m_OpenArchiveFile  = ( OpenArchiveFile ) unitsynclib->resolve( "OpenArchiveFile" );
    m_ReadArchiveFile  = ( ReadArchiveFile ) unitsynclib->resolve( "ReadArchiveFile" );
    m_CloseArchiveFile  = ( CloseArchiveFile ) unitsynclib->resolve( "CloseArchiveFile" );
    m_SizeArchiveFile  = ( SizeArchiveFile ) unitsynclib->resolve( "SizeArchiveFile" );
    m_GetSpringConfigString  = ( GetSpringConfigString ) unitsynclib->resolve( "GetSpringConfigString" );
    m_GetSpringConfigInt  = ( GetSpringConfigInt ) unitsynclib->resolve( "GetSpringConfigInt" );
    m_GetSpringConfigFloat  = ( GetSpringConfigFloat ) unitsynclib->resolve( "GetSpringConfigFloat" );
    m_SetSpringConfigString  = ( SetSpringConfigString ) unitsynclib->resolve( "SetSpringConfigString" );
    m_SetSpringConfigInt  = ( SetSpringConfigInt ) unitsynclib->resolve( "SetSpringConfigInt" );
    m_SetSpringConfigFloat  = ( SetSpringConfigFloat ) unitsynclib->resolve( "SetSpringConfigFloat" );

    m_Init( 0, 0 );
    library_loaded = true;
#ifdef Q_WS_WIN
    /* Change current dir back to default */
    QDir::setCurrent(originaldir);
#endif
    return true;
}

UnitSyncLib::~UnitSyncLib() {
}

void UnitSyncLib::TestCall() {
    NON_REENTRANT;
    if ( !library_loaded )
        return;
    qDebug() << tr("UnitSyncLib was compiled against Spring version: ") << m_GetSpringVersion();
    int mapcount = m_GetMapCount();
    int modcount = m_GetPrimaryModCount();
    qDebug() << tr("Found %1 maps and %2 mods.").arg(mapcount).arg(modcount);
//    for ( int i = 0; i < mapcount; ++i ) {
//        unsigned int checksum = m_GetMapChecksum( i );
//        QString mapName = m_GetMapName( i );
//        qDebug() << "  " <<  i << " mapName: " << mapName << ", checksum: " << checksum;
//    }
//    for ( int i = 0; i < modcount; ++i ) {
//        qDebug() << "  " << i << " modName " << m_GetPrimaryModName( i ) << m_GetPrimaryModChecksum( i );
//        qDebug() << "        based on: " << m_GetPrimaryModArchive( i );
//    }
}

QImage UnitSyncLib::getMinimapQImage( const QString mapFileName, int miplevel, bool scaled ) {
    NON_REENTRANT;
    if ( !libraryLoaded() )
        return QImage();
    const unsigned int height = 1 << ( 10 - miplevel );
    const unsigned int width  = 1 << ( 10 - miplevel );
    QByteArray map(( char* ) m_GetMinimap( mapFileName.toAscii(), miplevel ), width*height*2 );
    QImage mapImage(( uchar* ) map.constData(), width, height, QImage::Format_RGB16 );
    if (scaled) {
        int height;
        int width;
        m_GetInfoMapSize(mapFileName.toAscii(), "height", &width, &height);
        mapImage = mapImage.scaled(width, height);
    }
    return mapImage.copy();
}

QImage UnitSyncLib::getHeightMapQImage( const QString mapFileName ) {
    NON_REENTRANT;
    if ( !libraryLoaded() )
        return QImage();
    int height;
    int width;
    const int HueLow = 210;
    const int HueHigh = 0;
    int shortMax = 65535;
    m_GetInfoMapSize(mapFileName.toAscii(), "height", &width, &height);
    unsigned short *ptr = new unsigned short[width*height];
    unsigned int *rgb = new unsigned int[width*height];
    m_GetInfoMap(mapFileName.toAscii(), "height", ptr, 2);
    for (int i = 0; i < width * height; i++) {
        rgb[i] = QColor::fromHsv(ptr[i]/(float)shortMax*(HueHigh-HueLow)+HueLow, 255, ptr[i]/(float)shortMax*155+100).rgb();
    }
    QByteArray map((char*)rgb, width*height*4);
    QImage mapImage(( uchar* ) map.constData(), width, height, QImage::Format_RGB32 );
    delete[] ptr;
    delete[] rgb;
    return mapImage.copy();
}

RawHeightMap UnitSyncLib::getHeightMapRaw( const QString mapFileName ) {
    NON_REENTRANT;
    if ( !libraryLoaded() )
        return RawHeightMap(0,0,0);
    int height;
    int width;
    m_GetInfoMapSize(mapFileName.toAscii(), "height", &width, &height);
    unsigned short *ptr = new unsigned short[width*height];
    m_GetInfoMap(mapFileName.toAscii(), "height", ptr, 2);
    return RawHeightMap(width,height,ptr);
}

QImage UnitSyncLib::getMetalMapQImage( const QString mapFileName ) {
    NON_REENTRANT;
    if ( !libraryLoaded() )
        return QImage();
    int height;
    int width;
    m_GetInfoMapSize(mapFileName.toAscii(), "metal", &width, &height);
    unsigned char *ptr = new unsigned char[width*height];
    unsigned int *rgb = new unsigned int[width*height];
    m_GetInfoMap(mapFileName.toAscii(), "metal", ptr, 1);
    for (int i = 0; i < width * height; i++) {
        rgb[i] = QColor::fromHsv(150, 255, ptr[i]).rgb();
    }
    QByteArray map((char*)rgb, width*height*4);
    QImage mapImage(( uchar* ) map.constData(), width, height, QImage::Format_RGB32 );
    delete[] ptr;
    delete[] rgb;
    return mapImage.copy();
}

void UnitSyncLib::getMapInfo(QString mapFileName, MapInfo* info) {
    NON_REENTRANT;
    if (!libraryLoaded()) return;
    m_GetMapInfoEx(mapFileName.toAscii(), info, 1);
    //qDebug() << "UNITSYNC_DUMP: " << "GetMapInfoEx";
}

unsigned int UnitSyncLib::mapChecksum( QString mapName ) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetMapChecksumFromName";
    return libraryLoaded() ? m_GetMapChecksumFromName( mapName.toAscii() ) : 0;
}

unsigned int UnitSyncLib::modChecksum( QString modName ) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetPrimaryModChecksumFromName";
    return libraryLoaded() ? m_GetPrimaryModChecksumFromName( modName.toAscii() ) : 0;
}

signed int UnitSyncLib::modIndex( QString modName ) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetPrimaryModInde";
    return libraryLoaded() ? m_GetPrimaryModIndex( modName.toAscii() ) : -1;
}

signed int UnitSyncLib::mapIndex( QString mapName ) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetMapArchiveCount";
    return libraryLoaded() ? m_GetMapArchiveCount( mapName.toAscii() ) : -1;
}

QString UnitSyncLib::modArchive( int modIndex ) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetPrimaryModCount";
    return libraryLoaded() && modIndex < m_GetPrimaryModCount() ? QString( m_GetPrimaryModArchive( modIndex ) ) : "";
}

bool UnitSyncLib::setCurrentMod(QString modname) {
    NON_REENTRANT;
    if (!libraryLoaded()) return false;
    if (m_currentModName == modname) return true;
    //qDebug() << "UNITSYNC_DUMP: " << "GetPrimaryModIndex";
    int index = m_GetPrimaryModIndex(modname.toAscii());
    if (index < 0) return false;
    //qDebug() << "UNITSYNC_DUMP: " << "RemoveAllArchives";
    //qDebug() << "UNITSYNC_DUMP: " << "GetPrimaryModArchive";
    //qDebug() << "UNITSYNC_DUMP: " << "AddAllArchives";

    m_RemoveAllArchives();
    m_AddAllArchives(m_GetPrimaryModArchive(index));
    m_currentModName = modname;
    m_sideIconsCache.clear();
    m_sideNamesCache.clear();
    return true;
}

QIcon UnitSyncLib::getSideIcon(QString sidename) {
    if (m_sideIconsCache.contains(sidename))
        return m_sideIconsCache[sidename];
    if (sidename.isEmpty()) return QIcon();
    NON_REENTRANT;
    // Maybe some cleaning needed here (aj)
    // Done (ko)
    if (!libraryLoaded()) return QIcon();

    QString filepath = QString("SidePics/%1.bmp").arg(sidename.toUpper());
    //qDebug() << "UNITSYNC_DUMP: " << "OpenFileVFS";
    int ret = m_OpenFileVFS(filepath.toAscii());
    if (ret < 0) return QIcon();
    //qDebug() << "UNITSYNC_DUMP: " << "FileSizeVFS";
    int filesize = m_FileSizeVFS(ret);
    if (filesize == 0) return QIcon();

    QByteArray sideicon;
    sideicon.resize(filesize);
    //qDebug() << "UNITSYNC_DUMP: " << "ReadFileVFS";
    m_ReadFileVFS(ret, sideicon.data(), filesize);

    QPixmap rawicon;
    rawicon.loadFromData(sideicon);
    QIcon ico = QIcon(rawicon);
    m_sideIconsCache[sidename] = ico;
    return ico;
}

QString UnitSyncLib::getSpringVersion() {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetSpringVersion";
    return libraryLoaded() ? QString(m_GetSpringVersion()) : "";
}

int UnitSyncLib::getSideNameCount() {
    if (m_currentModName.isEmpty()) return 0;
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetSideCount";
    return libraryLoaded() ? m_GetSideCount( ) : -1;
}

QString UnitSyncLib::sideName( int index ) {
    if (m_sideNamesCache.contains(index))
        return m_sideNamesCache[index];
    if (m_currentModName.isEmpty()) return QString();
    NON_REENTRANT;
    if ( libraryLoaded() ) {
        //qDebug() << "UNITSYNC_DUMP: " << "GetSideCount";
        if ( m_GetSideCount(  ) > index ) {
            //qDebug() << "UNITSYNC_DUMP: " << "GetSideName";
            QString name = m_GetSideName( index );
            m_sideNamesCache[index] = name;
            return name;
        }
    }
    return "";
}

bool UnitSyncLib::libraryLoaded() {
    if ( !library_loaded ) {
        qDebug() << tr("Your library is not loaded, you can't use it!");
        return false;
    }
    return true;
}

int UnitSyncLib::getModOptionCount() {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetModOptionCount";
    return m_GetModOptionCount();
}

OptionType UnitSyncLib::getOptionType(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionType";
    int type = m_GetOptionType(optIndex);
    switch (type) {
    case 1:
        return OT_BOOLEAN;
    case 2:
        return OT_LIST;
    case 3:
        return OT_FLOAT;
    case 4:
        return OT_STRING;
    case 5:
        return OT_SECTION;
    default:
        return OT_UNDEFINED;
    }
}

QString UnitSyncLib::getOptionName(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionName";
    return m_GetOptionName(optIndex);
}

QString UnitSyncLib::getOptionSection(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionSection";
    return m_GetOptionSection(optIndex);
}

QString UnitSyncLib::getOptionKey(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionKey";
    return m_GetOptionKey(optIndex);
}

bool UnitSyncLib::isGameOption(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionKey";
    return m_gameOptionKeys.contains(m_GetOptionKey(optIndex));
}

bool UnitSyncLib::isGameOption(QString key) {
    return m_gameOptionKeys.contains(key);
}

bool UnitSyncLib::getOptionBoolDef(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionBoolDef";
    return m_GetOptionBoolDef(optIndex);
}

QString UnitSyncLib::getOptionListDef(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionListDef";
    return m_GetOptionListDef(optIndex);
}

float UnitSyncLib::getOptionNumberDef(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionNumberDef";
    return m_GetOptionNumberDef(optIndex);
}

float UnitSyncLib::getOptionNumberMin (int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionNumberMin";
    return m_GetOptionNumberMin(optIndex);
}

float UnitSyncLib::getOptionNumberMax (int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionNumberMax";
    return m_GetOptionNumberMax(optIndex);
}

float UnitSyncLib::getOptionNumberStep (int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionNumberStep";
    return m_GetOptionNumberStep(optIndex);
}

QString UnitSyncLib::getOptionStringDef(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionStringDef";
    return m_GetOptionStringDef(optIndex);
}

int UnitSyncLib::getOptionStringMaxLen(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionStringMaxLen";
    return m_GetOptionStringMaxLen(optIndex);
}

void UnitSyncLib::reboot() {
    MANUAL_LOCK;
    if (library_loaded) {
        //qDebug() << "UNITSYNC_DUMP: " << "UnInit";
        m_RemoveAllArchives();
        //qDebug() << "UNITSYNC_DUMP: " << "RemoveAllArchives";
        m_UnInit();
        //qDebug() << "UNITSYNC_DUMP: " << "Init";
        m_Init(0,0);
        //qDebug() << "UNITSYNC_DUMP: " << "GetPrimaryModIndex";
        int index = m_GetPrimaryModIndex(m_currentModName.toAscii());
        if (index >= 0) {
            //qDebug() << "UNITSYNC_DUMP: " << "GetPrimaryModArchive";
            //qDebug() << "UNITSYNC_DUMP: " << "AddAllArchives";
            m_AddAllArchives(m_GetPrimaryModArchive(index));
        }
        MANUAL_UNLOCK;
        initMapNamesCache();
        initModNamesCache();
        emit rebooted();
    }
}

void UnitSyncLib::initMapNamesCache() {
    m_mapNamesCache.clear();
    if( library_loaded ) {
        int mapcount = m_GetMapCount();
        for ( int i = 0; i < mapcount; ++i ) {
            m_mapNamesCache << m_GetMapName(i);
        }
    }
}

void UnitSyncLib::initModNamesCache() {
    m_modNamesCache.clear();
    if( library_loaded ) {
        int modcount = m_GetPrimaryModCount();
        for ( int i = 0; i < modcount; ++i ) {
            m_modNamesCache << m_GetPrimaryModName(i);
        }
    }
}

QSet<QString> UnitSyncLib::getModNames() {
    return m_modNamesCache;
}

QSet<QString> UnitSyncLib::getMapNames() {
    return m_mapNamesCache;
}

QStringList UnitSyncLib::getOptionListItems(int optIndex) {
    QStringList ret;
    if (library_loaded) {
        int count = m_GetOptionListCount(optIndex);
        for(int i = 0; i < count; i++) {
            ret << m_GetOptionListItemKey(i, optIndex);
        }
    }
    return ret;
}
