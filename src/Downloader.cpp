// $Id: InfoChannel.cpp 229 2009-07-23 02:18:19Z mirko.windhoff $
// QtLobby released under the GPLv3, see COPYING for details.
#include "Downloader.h"
#include <QUrl>
#include <QDebug>
#include <QApplication>
#include <QTimer>
#include "Settings.h"

//constants
const QString resolverUrl = "http://134.2.74.148:8080/qfc/map/%1";
const QString mirrorListUrl = "http://spring.jobjol.nl/checkmirror.php?q=%1&c=%2";
const QString userAgent = "Mozilla/5.0 (Windows; U; Windows NT 6.0; en-US; rv:1.9.1.1) Gecko/20090715 Firefox/3.5.1";
const QString referrer = "http://spring.jobjol.nl";
const int testChunk = 102400; //100 Kb

/**
Ctor. Gets resource name and resource type
*/
Downloader::Downloader(QString name, bool map, QObject* parent) : QObject(parent) {
    m_resourceName = name;
    m_map = map;
    m_manager = 0;
    m_received = 0;
    m_downloading = false;
    m_mirrorList.setRawHeader("User-Agent", userAgent.toAscii());
    m_mirrorList.setRawHeader("Referer", referrer.toAscii());
    //qDebug() << "ctor, name=" + name;
}

Downloader::~Downloader() {
    if(m_manager) delete m_manager;
}

/**
 main function.
*/
void Downloader::start() {
    m_downloading = false;
    //qDebug() << "run, name=" + m_resourceName;
    m_manager = new QNetworkAccessManager(this);
    m_resolve.setUrl(resolverUrl.arg(m_resourceName));
    m_reply = m_manager->get(m_resolve);
    connect(m_reply, SIGNAL(finished()), this, SLOT(onResolverFinished()));
    emit stateChanged(m_resourceName, "Resolving filename");
}

/**
Slot for getting resource filename from resource name via Mirko's gateway
*/
void Downloader::onResolverFinished() {
    m_fileName = m_reply->readAll().trimmed();
    if(m_fileName.isEmpty()) {
        emit stateChanged(m_resourceName, "File not found");
        emit finished(m_resourceName, false);
        return;
    }
    //qDebug() << "onResolverFinished, filename=" + m_fileName;
    QString url;
    if(m_map)
        url = mirrorListUrl.arg(m_fileName, "maps");
    else
        url = mirrorListUrl.arg(m_fileName, "mods");
    m_mirrorList.setUrl(url);
    m_reply = m_manager->get(m_mirrorList);
    connect(m_reply, SIGNAL(finished()), this, SLOT(onMirrorListFinished()));
    emit stateChanged(m_resourceName, "Retrieving mirror list");
}

/**
Slot for getting mirror list from jobjol.nl
*/
void Downloader::onMirrorListFinished() {
    QString mirrors = m_reply->readAll();
    if(mirrors.isEmpty()) {
        emit stateChanged(m_resourceName, "Faied to retrieve mirror list");
        emit finished(m_resourceName, false);
        return;
    }
    QRegExp re("mirror=(http%3A%2F%2F.*)&");
    re.setMinimal(true);
    int idx = 0;
    while((idx = re.indexIn(mirrors, idx)) > 0) {
        m_mirrors << QUrl::fromPercentEncoding(re.cap(1).toAscii());
        idx += re.cap(0).length();
    }
    if(m_mirrors.size()) {
        QNetworkRequest request = QNetworkRequest(m_mirrors.at(0));
        QNetworkReply* reply = m_manager->get(request);
        connect(reply, SIGNAL(metaDataChanged()), this, SLOT(onFileSizeFinished()));
    }
    //qDebug() << "onMirrorListFinished, mirrors=" << m_mirrors;
    emit stateChanged(m_resourceName, "Retrieving file size");
}

/**
Slot for getting size of a file. It aborts connection as soon as we got it
*/
void Downloader::onFileSizeFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    m_fileSize = reply->header(QNetworkRequest::ContentLengthHeader).toUInt();
    //qDebug() << "onFileSizeFinished, fileSize=" << m_fileSize;
    reply->abort();
    foreach(QString mirror, m_mirrors) {
        QNetworkRequest request = QNetworkRequest(mirror);
        request.setRawHeader("Range", ("bytes=0-"+QString::number(testChunk)).toAscii());//download testChunk bytes
        m_requests << request;
        m_hostSpeeds << 0;
        m_recievedList << 0;
        QNetworkReply* reply = m_manager->get(request);
        QTime t;
        t.start();
        m_times << t;
        connect(reply, SIGNAL(finished()), this, SLOT(onFetchFinished()));
        m_replies << reply;
    }
    QTimer::singleShot(15000, this, SLOT(download())); //start download anyway in 10 seconds if one ore more of mirrors failed
    emit stateChanged(m_resourceName, "Estimating mirrors speeds");
}

/**
Slot for getting requested resource
*/
void Downloader::onFetchFinished() {
    static int n = 0;
    if(m_downloading) return;
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    int idx = m_replies.indexOf(reply);
    m_hostSpeeds[idx] = qRound(testChunk/m_times[idx].elapsed());
    //qDebug() << "onFetchFinished, hostSpeed=" << m_hostSpeeds[idx];
    n++;
    if(n == m_requests.size()) {
        m_downloadSources = 0;
        download();
        n = 0;
    }
}

/**
This method starts multisegmented download
*/
void Downloader::download() {
    if(m_downloading) return;
    m_downloading = true;
    QString dir = Settings::Instance()->value("spring_user_dir").toString();
    m_out.setFileName(dir + "/" + (m_map ? "maps" : "mods") + "/" + m_fileName);
    if(!m_out.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to create file";
        return;
    }
    //qDebug() << "download, fileName=" << m_out.fileName();
    m_times.clear();
    m_ranges = getChunkRanges(m_hostSpeeds, m_fileSize);
    for(int i = 0; i < m_requests.size(); i++) {
        QNetworkRequest& request = m_requests[i];
        if(m_ranges[i][1]) {
            QString range = "bytes=%1-%2";
            request.setRawHeader("Range", range.arg(m_ranges[i][0]).arg(m_ranges[i][0]+m_ranges[i][1]).toAscii());
            QNetworkReply* reply = m_manager->get(request);
            connect(reply, SIGNAL(finished()), this, SLOT(onDownloadFinished()));
            connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
                    this, SLOT(onDownloadProgress(qint64,qint64)));
            m_replies[i] = reply;
            m_downloadSources++;
        }
    }
    emit stateChanged(m_resourceName, QString("Downloading from %1 mirror(s)").arg(m_downloadSources));
}

/**
Slot for processing completed segment
*/
void Downloader::onDownloadFinished() {
    static int n = 0;
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    int idx = m_replies.indexOf(reply);
    QByteArray data = reply->readAll();
    qDebug() << "onDownloadFinished, offset=" << m_ranges[idx][0];
    m_out.seek(m_ranges[idx][0]);
    m_out.write(data);
    n++;
    if(n == m_downloadSources) {
        n = 0;
        m_out.close();
        emit stateChanged(m_resourceName, "Finished");
        emit finished(m_resourceName, true);
    }
}

/**
Informing the world about how we are doing
*/
void Downloader::onDownloadProgress(qint64 bytesReceived, qint64 /*bytesTotal*/) {
    //Probably it needs some optimizations concerning index lookup. (ko)
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    int idx = m_replies.indexOf(reply);
    m_recievedList[idx] = bytesReceived;
    m_received = 0;
    foreach(quint64 r, m_recievedList)
        m_received += r;
    emit downloadProgress(m_resourceName, m_received, m_fileSize);
}

/**
  Gives the startpoint and number of bytes to download for downloading for each host.
  @param hostSpeed is a list of speeds in Bytes/s
  @param fileSize is the size of the file to download
  @return is a list of start position and number of Bytes to download from each host (might be 0 for skipping that host).
*/
QVector<QVector<int> > Downloader::getChunkRanges( QVector<int> hostSpeed, int fileSize ) {
    int maxSpeed = 0, sumSpeed = 0;
    QVector<float> fractions;
    foreach( int speed, hostSpeed ) {
        fractions.append((float) speed);
        maxSpeed = qMax( maxSpeed, speed );
        sumSpeed += speed;
    }
    float sumFraction =0;
    for( int i = 0; i < fractions.size(); i++ ) {
        fractions[i] = fractions[i]/sumSpeed; // get
        if( hostSpeed.at(i) == maxSpeed )
            fractions[i] *= 1.2; // give the fastest a little bit more than it's proportional part
        if( hostSpeed.at(i) < maxSpeed*0.2 )
            fractions[i] = 0; // don't download from too slow servers
        sumFraction += fractions[i];
    }
    QVector<QVector<int> > ret;
    int currentPos = 0;
    foreach( float fraction, fractions ) {
        QVector<int> entry;
        int startPos = currentPos;
        int bytes = (int) qRound(fraction/sumFraction*fileSize);
        currentPos += bytes;
        entry.append(startPos);
        entry.append(bytes);
        ret.append(entry);
    }
    ret.last()[1] += fileSize - currentPos; // due to round some bytes might be left, assign to last one
    return ret;
}
