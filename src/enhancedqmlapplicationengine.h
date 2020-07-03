#pragma once

#include <QFileSystemWatcher>
#include <QQmlApplicationEngine>

class EnhancedQmlApplicationEngine : public QQmlApplicationEngine
{
  Q_OBJECT
public:
  explicit EnhancedQmlApplicationEngine(QObject* parent = nullptr);

  Q_INVOKABLE QUrl mainUrl() const;
  void setMainUrl(const QUrl& mainUrl);

public slots:
  Q_INVOKABLE void clearCache(const QString& path = "");
  void refreshWatcher();

  void load(QUrl file);
signals:
  void refresh();

private:
  QFileSystemWatcher mFileWatcher;
  QUrl m_mainUrl;
};
