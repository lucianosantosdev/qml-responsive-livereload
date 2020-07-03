#include "mainwindow.h"
#include "enhancedqmlapplicationengine.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuick/QQuickWindow>
#include <QtWidgets/QApplication>
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , m_layout(new QHBoxLayout)
{
  ui->setupUi(this);
  setWindowTitle("Qml Responsive Livereload");
  ui->widget->setLayout(m_layout);

  appendNewWindow(320, 240);
  appendNewWindow(240, 320);
  appendNewWindow(480, 800);

  QSettings settings;
  QUrl qmlUrl = settings.value("qmlApplicationFolder").toUrl();
  if (!qmlUrl.isValid()) {
    on_actionOpen_folder_triggered();
  } else {
    loadQml(qmlUrl);
  }
}

void
MainWindow::appendNewWindow(int width, int height)
{
  QSharedPointer<EnhancedQmlApplicationEngine> engine(
    new EnhancedQmlApplicationEngine);
  engine->load(QUrl(QStringLiteral("qrc:/main.qml")));

  QWindow* w = qobject_cast<QWindow*>(engine->rootObjects().at(0));
  w->resize(width, height);

  QWidget* container = QWidget::createWindowContainer(w);
  container->setFixedSize(w->size());
  container->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  m_layout->addWidget(container);
  m_qmlAppEngineList.push_back(engine);

  w->installEventFilter(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

bool
MainWindow::eventFilter(QObject* watched, QEvent* event)
{
  QWindow* watchedWindow = dynamic_cast<QWindow*>(watched);
  if (watchedWindow == nullptr) {
    return QMainWindow::eventFilter(watched, event);
  }

  for (auto engine : m_qmlAppEngineList) {
    QWindow* w = qobject_cast<QWindow*>(engine->rootObjects().at(0));
    if (w != watched) {
      QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
      if (mouseEvent != nullptr) {

        float x =
          (mouseEvent->x() / (float)watchedWindow->width()) * w->size().width();
        float y = (mouseEvent->y() / (float)watchedWindow->height()) *
                  w->size().height();

        QPointF newPos(x, y);
        QMouseEvent eventScaled(mouseEvent->type(),
                                newPos,
                                mouseEvent->button(),
                                mouseEvent->buttons(),
                                mouseEvent->modifiers());
        w->removeEventFilter(this);
        QApplication::sendEvent(w, &eventScaled);
        w->installEventFilter(this);
        update();
      } else {
      }
    }
  }
  return QMainWindow::eventFilter(watched, event);
}

void
MainWindow::on_actionOpen_folder_triggered()
{
  QUrl qmlUrl =
    QFileDialog::getOpenFileUrl(this, tr("Open File"), tr("Qml (qml.png)"));

  if (qmlUrl.isValid()) {
    QSettings settings;
    settings.setValue("qmlApplicationFolder", qmlUrl);
    loadQml(qmlUrl);
  }
}

void
MainWindow::loadQml(const QUrl& qmlUrl)
{
  QString fileDir = QFileInfo(qmlUrl.toLocalFile()).absolutePath();
  qputenv("QT_QUICK_CONTROLS_CONF",
          (fileDir + "/qtquickcontrols2.conf").toLatin1());
  for (auto& qmlAppEngine : m_qmlAppEngineList) {
    qmlAppEngine->setMainUrl(qmlUrl);
  }
}
