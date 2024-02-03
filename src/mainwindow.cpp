#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Analyzewindow.h"


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    move(QGuiApplication::screens().at(0)->geometry().center() - frameGeometry().center());
    setAcceptDrops(true);
}


MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::dragEnterEvent(QDragEnterEvent *e) {
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}


void MainWindow::dropEvent(QDropEvent *e) {
    foreach(const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        QFileInfo fileInfo(fileName);
        if (fileInfo.suffix().toLower() == "pcap") {
            qDebug() << "Dropped file:" << fileName;
            pcapVector.push_back(fileName);
        } else {
            QMessageBox::about(this, "WARNING", "Check extension is \'.pcap\'");
        }
    }
}


void MainWindow::processPcapFile(const int i, const std::string &pcapFile,
                                 PcapReader &PR,
                                 SnortRunner &SR) {
    SR.generateSnortLog(pcapFile);
    if (PR.open(pcapFile))
        PR.readPcap(pcapFile);
    else
        QMessageBox::warning(this,
                             "Error",
                             "Something goes wrong while reading the file");
}


void MainWindow::on_submitBtn_clicked() {
    unsigned int fileCount = pcapVector.size();
    if (!fileCount) {
        QMessageBox::about(this, "WARNING", "No file detected");
    } else {
        AnalyzeWindow analyzeWindow[fileCount];
        auto &pcapReader = reinterpret_cast<PcapReader &>(PcapReader::getInstance());
        auto &snortRunner = reinterpret_cast<SnortRunner &>(SnortRunner::getInstance());

        for (int i = 0; i < fileCount; i++) {
            const std::string pcapFile = pcapVector[i].toUtf8().constData();
            processPcapFile(i, pcapFile, pcapReader, snortRunner);

            analyzeWindow[i].setModal(true);
            analyzeWindow[i].exec();
        }
    }
}

