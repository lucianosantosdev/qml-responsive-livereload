#pragma once

#include <QHBoxLayout>
#include <QMainWindow>
#include <QPainter>
#include <QQmlApplicationEngine>

class EnhancedQmlApplicationEngine;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

public:
  bool eventFilter(QObject* watched, QEvent* event);

private slots:
  void on_actionOpen_folder_triggered();

private:
  void appendNewWindow(int width, int height);
  void loadQml(const QUrl& qmlUrl);

  Ui::MainWindow* ui;
  QHBoxLayout* m_layout;
  QList<QSharedPointer<EnhancedQmlApplicationEngine>> m_qmlAppEngineList;
};
