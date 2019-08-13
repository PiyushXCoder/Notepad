#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    #if !QT_CONFIG(printer)
        ui->actionPrint->setEnabled(false);
    #endif

    #if !QT_CONFIG(clipboard)
        ui->actionCut->setEnabled(false);
        ui->actionCopy->setEnabled(false);
        ui->actionPaste->setEnabled(false);
    #endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    currentFile.clear();
    ui->textEdit->setText(QString());
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open File");
    if(filename.isEmpty()) return;
    QFile file(filename);
    currentFile = filename;

    if(!file.open(QIODevice::ReadOnly)){
        ui->statusBar->showMessage("Can't open file: "+file.errorString(), 3000);
    }

    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
    this->setWindowTitle(QFileInfo(filename).fileName()+" - Notepad");
}

bool MainWindow::on_actionSave_triggered()
{
    QFile file(currentFile);
    if(!file.exists())
    {
        this->setWindowTitle("Notepad");
        QString filename = QFileDialog::getSaveFileName(this, "Save");
        if(filename.isEmpty()) return false;
        file.setFileName(filename);
        currentFile = filename;
        this->setWindowTitle(QFileInfo(filename).fileName()+" - Notepad");
    }

    if(!file.open(QIODevice::WriteOnly)){
        ui->statusBar->showMessage("Can't save file: "+file.errorString(), 3000);
        return false;
    }

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;

    file.close();

    return true;
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString tmp = currentFile;
    currentFile.clear();

    if(!on_actionSave_triggered())
    {
        this->setWindowTitle(QFileInfo(tmp).fileName()+" - Notepad");
        currentFile = tmp;
    }
}


void MainWindow::on_actionPrint_triggered()
{
    #if QT_CONFIG(printer)
        QPrinter printDev;
    #if QT_CONFIG(printdialog)
        QPrintDialog dialog(&printDev, this);
        if (dialog.exec() == QDialog::Rejected)
            return;
    #endif // QT_CONFIG(printdialog)
        ui->textEdit->print(&printDev);
    #endif // QT_CONFIG(printer)
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Developed by PiyushXCoder");
    msgBox.exec();
}
