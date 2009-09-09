#include "PathManager.h"

#include <QFile>
#include <QDir>
#include <QDebug>

PathManager::PathManager() {
}

void PathManager::setResourceRoot(const QString& path) {
    m_base = path + QDir::separator();
}

void PathManager::setOverlayPath(const QString& path) {
    m_overlay = path + QDir::separator();
}

QString PathManager::resolvePath(const QString& path) {
    if(!m_inOverlay.contains(path))
        m_inOverlay[path] = QFile::exists(m_overlay+path);
    if(m_inOverlay[path])
        return m_overlay + path;
    else
        return m_base + path;
}

QString PathManager::resolveOverlayPath(const QString& path) {
    return m_overlay + path;
}

QString PathManager::resolveOverlayPath() {
    return m_overlay;
}

void PathManager::invalidateCache(const QString& path) {
    m_inOverlay[path] = QFile::exists(m_overlay+path);
}

