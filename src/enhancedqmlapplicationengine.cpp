#include "enhancedqmlapplicationengine.h"
#include <QTimer>

#include <QDir>
#include <QFile>
#include <QQmlContext>
#include <QtDebug>

EnhancedQmlApplicationEngine::EnhancedQmlApplicationEngine(QObject* parent)
  : QQmlApplicationEngine(parent)
{
  refreshWatcher();
  connect(&mFileWatcher, &QFileSystemWatcher::fileChanged, this, [this]() {
    emit refresh();
  });
}

void
EnhancedQmlApplicationEngine::clearCache(const QString& path)
{
  (void)path;
  this->clearComponentCache();
  refreshWatcher();
}

void
EnhancedQmlApplicationEngine::refreshWatcher()
{
  QDir dir(m_mainUrl.toLocalFile());
  dir.cdUp();
  auto fileList = dir.entryList(QStringList() << "*.qml");
  for (auto file : fileList) {
    mFileWatcher.addPath(dir.path() + "/" + file);
  }
}

void
EnhancedQmlApplicationEngine::load(QUrl file)
{
  rootContext()->setContextProperty("$QmlEngine", this);

  QQmlApplicationEngine::load(file);

  QDir dir(file.toLocalFile());
  dir.cdUp();
  auto fileList = dir.entryList(QStringList() << "*.qml");
  for (auto file : fileList) {
    mFileWatcher.addPath(dir.path() + "/" + file);
  }

  m_mainUrl = file;
}

QUrl
EnhancedQmlApplicationEngine::mainUrl() const
{
  return m_mainUrl;
}

void
EnhancedQmlApplicationEngine::setMainUrl(const QUrl& mainUrl)
{
  m_mainUrl = mainUrl;
  emit refresh();
}
