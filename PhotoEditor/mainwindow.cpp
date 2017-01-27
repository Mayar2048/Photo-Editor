#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <iostream>
#include <rotdialog.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    originStack = new QStack<ImageStruct>;
    redoStack = new QStack<ImageStruct>;
    rotationAngle = 0;

    rubberBand = NULL;
    isSelect = false;

    ui->setupUi(this);
    createActions();
    disableActions();

    // QPixmap pix(":/img/no_photo.jpg");
    // ui->imageLabel->setPixmap(pix);
  //  ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->imageLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->imageLabel->setBackgroundRole(QPalette::Base);
    ui->imageLabel->setScaledContents(false);
    ui->scrollArea->setBackgroundRole(QPalette::Base);
    setCentralWidget(ui->scrollArea);
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ... GUI ... ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void MainWindow::createActions(){

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ FILE MENU ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
    fileToolBar->setMovable(false);

    const QIcon openIcon = QIcon::fromTheme("image-open", QIcon(":/img/open.png"));
    openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    const QIcon saveAsIcon = QIcon::fromTheme("image-save-as", QIcon(":/img/save.png"));
    saveAsAct = new QAction(saveAsIcon, tr("&Save"), this);
    saveAsAct->setShortcuts(QKeySequence::Save);
    saveAsAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::save_as);
    fileMenu->addAction(saveAsAct);
    fileToolBar->addAction(saveAsAct);

    fileMenu->addSeparator();

    const QIcon exitIcon = QIcon::fromTheme("image-exit", QIcon(":/img/exit.png"));
    exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ EDIT MENU ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));
    editToolBar->setMovable(false);

    const QIcon undoIcon = QIcon::fromTheme("image-undo", QIcon(":/img/undo.png"));
    undoAct = new QAction(undoIcon, tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo"));
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);
    editMenu->addAction(undoAct);
    editToolBar->addAction(undoAct);

    const QIcon redoIcon = QIcon::fromTheme("image-redo", QIcon(":/img/redo.png"));
    redoAct = new QAction(redoIcon, tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo"));
    connect(redoAct, &QAction::triggered, this, &MainWindow::redo);
    editMenu->addAction(redoAct);
    editToolBar->addAction(redoAct);

    editMenu->addSeparator();

    const QIcon zoomInIcon = QIcon::fromTheme("image-zoom-in", QIcon(":/img/zoom_in.png"));
    zoomInAct = new QAction(zoomInIcon, tr("&Zoom In"), this);
    zoomInAct->setShortcuts(QKeySequence::ZoomIn);
    zoomInAct->setStatusTip(tr("Zoom In ..."));
    connect(zoomInAct, &QAction::triggered, this, &MainWindow::zoom_in);
    editMenu->addAction(zoomInAct);
    editToolBar->addAction(zoomInAct);

    const QIcon zoomOutIcon = QIcon::fromTheme("image-zoom-out", QIcon(":/img/zoom_out.png"));
    zoomOutAct = new QAction(zoomOutIcon, tr("&Zoom Out"), this);
    zoomOutAct->setShortcuts(QKeySequence::ZoomOut);
    zoomOutAct->setStatusTip(tr("Zoom Out ..."));
    connect(zoomOutAct, &QAction::triggered, this, &MainWindow::zoom_out);
    editMenu->addAction(zoomOutAct);
    editToolBar->addAction(zoomOutAct);

    editMenu->addSeparator();

    const QIcon rotateIcon = QIcon::fromTheme("image-rotate", QIcon(":/img/rotate.png"));
    rotateAct = new QAction(rotateIcon, tr("&Rotate"), this);
    rotateAct->setStatusTip(tr("Rotate ..."));
    connect(rotateAct, &QAction::triggered, this, &MainWindow::rotate);
    editMenu->addAction(rotateAct);
    editToolBar->addAction(rotateAct);

    const QIcon resetIcon = QIcon::fromTheme("image-reset", QIcon(":/img/reset.png"));
    resetAct = new QAction(resetIcon, tr("&Reset"), this);
    resetAct->setStatusTip(tr("Reset ..."));
    connect(resetAct, &QAction::triggered, this, &MainWindow::reset);
    editMenu->addAction(resetAct);
    editToolBar->addAction(resetAct);

    const QIcon cropIcon = QIcon::fromTheme("image-crop", QIcon(":/img/crop.png"));
    cropAct = new QAction(cropIcon, tr("&crop"), this);
    cropAct->setStatusTip(tr("Reset ..."));
    connect(cropAct, &QAction::triggered, this, &MainWindow::crop);
    editMenu->addAction(cropAct);
    editToolBar->addAction(cropAct);

    editMenu->addSeparator();

    const QIcon selectIcon = QIcon::fromTheme("image-select", QIcon(":/img/select.png"));
    selectAct = new QAction(selectIcon, tr("&Select"), this);
    selectAct->setStatusTip(tr("Reset ..."));
    connect(selectAct, &QAction::triggered, this, &MainWindow::select);
    editMenu->addAction(selectAct);
    editToolBar->addAction(selectAct);


    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HELP MENU ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QToolBar *helpToolBar = addToolBar(tr("Help"));
    helpToolBar->setMovable(false);

    const QIcon aboutIcon = QIcon::fromTheme("image-about", QIcon(":/img/about.png"));
    aboutAct = new QAction(aboutIcon, tr("&About"), this);
    aboutAct->setStatusTip(tr("About ..."));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
    helpMenu->addAction(aboutAct);
    helpToolBar->addAction(aboutAct);
}

bool MainWindow::loadFile(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }
    setImage(newImage);
    setWindowFilePath(fileName);
    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth());
    statusBar()->showMessage(message);
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
    QImageWriter writer(fileName);

    if (!writer.write(image)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(), tr("Cannot write %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName)), writer.errorString());
        return false;
    }
    const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);
    return true;
}

void MainWindow::setImage(const QImage &newImage)
{
    originImage = newImage;
    image = newImage;
    sourceImage = newImage;
    enableActions();
    undoAct->setEnabled(false);
    redoAct->setEnabled(false);
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    ui->scrollArea->setVisible(true);
}

void MainWindow::scaleImage(double factor)
{
    Q_ASSERT(ui->imageLabel->pixmap());
    scaleFactor *= factor;

    QPixmap pixmap(*ui->imageLabel->pixmap());
    QImage img;
    img = pixmap.toImage();

    /*
    originStack->push(img);
    undoAct->setEnabled(true);
    redoAct->setEnabled(false);
    while(!redoStack->isEmpty()){
        redoStack->pop();
    }
    */

    ui->imageLabel->setPixmap(pixmap.scaled(factor * ui->imageLabel->pixmap()->width(), factor * ui->imageLabel->pixmap()->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    //ui->imageLabel->resize(scaleFactor * ui->imageLabel->pixmap()->size());
    adjustScrollBar(ui->scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(ui->scrollArea->verticalScrollBar(), factor);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void MainWindow::initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

void MainWindow::disableActions()
{
    saveAsAct->setEnabled(false);
    undoAct->setEnabled(false);
    redoAct->setEnabled(false);
    zoomInAct->setEnabled(false);
    zoomOutAct->setEnabled(false);
    rotateAct->setEnabled(false);
    resetAct->setEnabled(false);
    selectAct->setEnabled(false);
    cropAct->setEnabled(false);
    aboutAct->setEnabled(false);
}

void MainWindow::enableActions()
{
    openAct->setEnabled(true);
    saveAsAct->setEnabled(true);
    undoAct->setEnabled(false);
    redoAct->setEnabled(false);
    zoomInAct->setEnabled(true);
    zoomOutAct->setEnabled(true);
    rotateAct->setEnabled(true);
    resetAct->setEnabled(true);
    selectAct->setEnabled(true);
    cropAct->setEnabled(true);
    aboutAct->setEnabled(true);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ... SLOTS ... ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void MainWindow::open()
{
    if(rubberBand != NULL){
        rubberBand->hide();
    }
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);
    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void MainWindow::save_as()
{
    if(rubberBand != NULL){
        rubberBand->hide();
    }
    QFileDialog dialog(this, tr("Save File As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);
    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().first())) {}
}

void MainWindow::zoom_in()
{
    if(rubberBand != NULL){
        rubberBand->hide();
    }
    if(isSelect){
        isSelect = false;
        QRect rect = rubberBand->rect();
        if (rect.isEmpty() || rect.isNull() || rect.width() <= 10 || rect.height() <= 10){
            return;
        }
        float newScale = 1 / std::max(((float) rect.width() / ui->scrollArea->width()),
                                     ((float) rect.height() / ui->scrollArea->height()));
        scaleImage(newScale);
        float x  = (point.x() - ui->imageLabel->pos().x() - 12);
        float y = (point.y() - ui->imageLabel->pos().y() - 12);

        float widthRatio = (float) x / (ui->imageLabel->pixmap()->width());
        float hightRatio = (float) y / (ui->imageLabel->pixmap()->height());
        ui->scrollArea->verticalScrollBar()->setValue(hightRatio * ui->imageLabel->pixmap()->height());
        ui->scrollArea->horizontalScrollBar()->setValue(widthRatio * ui->imageLabel->pixmap()->width());
//        crop();
//        ui->imageLabel->setScaledContents(true);
    }else{
        scaleImage(1.25);
    }
}

void MainWindow::zoom_out()
{
    if(rubberBand != NULL){
        rubberBand->hide();
    }
    if(isSelect){
        isSelect = false;
        float x = point.x() - ui->imageLabel->pos().x() - 12;
        float y = point.y() - ui->imageLabel->pos().y() - 12;
        QRect rect = rubberBand->rect();
        if (rect.isEmpty() || rect.isNull() || rect.width() <= 10 || rect.height() <= 10)
                return;
        float newScale = std::max(((float) rect.width() / ui->scrollArea->width()),
                                     ((float) rect.height() / ui->scrollArea->height()));
        scaleImage(newScale);
        float widthRatio = (float) x / (ui->imageLabel->pixmap()->width());
        float hightRatio = (float) y / (ui->imageLabel->pixmap()->height());
        ui->scrollArea->verticalScrollBar()->setValue(hightRatio * ui->imageLabel->pixmap()->height());
        ui->scrollArea->horizontalScrollBar()->setValue(widthRatio * ui->imageLabel->pixmap()->width());
    }else{
        scaleImage(0.8);
    }
}

void MainWindow::rotate() {
    if(rubberBand != NULL){
        rubberBand->hide();
    }

    // displaying the rotation dialog
    RotDialog dialog;
    dialog.setModal(true);
    dialog.exec();

    ImageStruct imgStruct;
    QImage srcImage;
    QPixmap pixmap;
    QMatrix rm;

    srcImage = (originStack->isEmpty()) ? originImage : originStack->top().srcImg;
    pixmap = QPixmap::fromImage(srcImage);

    imgStruct.rotAngle = dialog.getDirection() * dialog.getAngle();
    rotationAngle += imgStruct.rotAngle;

    rm.translate(ui->imageLabel->width()/2, ui->imageLabel->height()/2);
    rm.rotate(rotationAngle);
    rm.translate(-ui->imageLabel->width()/2, -ui->imageLabel->height()/2);
    pixmap = pixmap.transformed(rm);

    ui->imageLabel->setPixmap(pixmap);
    ui->imageLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    imgStruct.srcImg = sourceImage;
    imgStruct.editedImg = pixmap.toImage();
    originStack->push(imgStruct);
    undoAct->setEnabled(true);
    redoAct->setEnabled(false);
    while(!redoStack->isEmpty()){
        redoStack->pop();
    }
}

void MainWindow::undo() {
    if(rubberBand != NULL){
        rubberBand->hide();
    }

    redoStack->push(originStack->pop());
    rotationAngle -= redoStack->top().rotAngle;
    redoAct->setEnabled(true);

    if(originStack->isEmpty()){
        undoAct->setEnabled(false);
        sourceImage = originImage;
        ui->imageLabel->setPixmap(QPixmap::fromImage(originImage));
    }else{
        ui->imageLabel->setPixmap(QPixmap::fromImage(originStack->top().editedImg));
    }
}

void MainWindow::redo() {
    if(rubberBand != NULL){
        rubberBand->hide();
    }

    originStack->push(redoStack->pop());
    ui->imageLabel->setPixmap(QPixmap::fromImage(originStack->top().editedImg));
    rotationAngle += originStack->top().rotAngle;
    if(redoStack->isEmpty()){
        redoAct->setEnabled(false);
    }
}

void MainWindow::reset(){
    if(rubberBand != NULL){
        rubberBand->hide();
    }

    enableActions();
    rotationAngle = 0;
    sourceImage = originImage;
    while(!originStack->isEmpty()){
        originStack->pop();
    }
    while(!redoStack->isEmpty()){
        redoStack->pop();
    }
    ui->imageLabel->setPixmap(QPixmap::fromImage(originImage));
}

void MainWindow::about()
{
    if(rubberBand != NULL){
        rubberBand->hide();
    }

    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
               "and QScrollArea to display an image. QLabel is typically used "
               "for displaying a text, but it can also display an image. "
               "QScrollArea provides a scrolling view around another widget. "
               "If the child widget exceeds the size of the frame, QScrollArea "
               "automatically provides scroll bars. </p><p>The example "
               "demonstrates how QLabel's ability to scale its contents "
               "(QLabel::scaledContents), and QScrollArea's ability to "
               "automatically resize its contents "
               "(QScrollArea::widgetResizable), can be used to implement "
               "zooming and scaling features. </p><p>In addition the example "
               "shows how to use QPainter to print an image.</p>"));
}

void MainWindow::select() {
    if(rubberBand != NULL){
        rubberBand->hide();
    }

    ui->scrollArea->setCursor(QCursor(Qt::CrossCursor));
    isSelect = true;
}

void MainWindow::crop() {
    if(isSelect){
        rubberBand->hide();
        QImage copyImage;
        QPixmap OriginalPix(*(ui->imageLabel->pixmap()));
        QRect rect(a, b);
        QImage newImage;
        newImage = OriginalPix.toImage();
        copyImage = newImage.copy(rect);
        sourceImage = copyImage;
        rotationAngle = 0;
        ui->imageLabel->setPixmap(QPixmap::fromImage(copyImage));
        ui->imageLabel->adjustSize();
        scaleFactor = 1.0;
        ui->imageLabel->resize(scaleFactor * ui->imageLabel->pixmap()->size());
        ui->imageLabel->adjustSize();
        ui->scrollArea->setBackgroundRole(QPalette::Base);
        // ui->scrollArea->setWidget(imageLabel);
        setCentralWidget(ui->scrollArea);
        rubberBand= NULL;
        isSelect = false;

        ImageStruct imgStruct;
        imgStruct.srcImg = imgStruct.editedImg = sourceImage;
        imgStruct.rotAngle = rotationAngle;
        originStack->push(imgStruct);
        undoAct->setEnabled(true);
        redoAct->setEnabled(false);
        while(!redoStack->isEmpty()){
            redoStack->pop();
        }
    }
}

QPoint pt;
QPoint diff;

void MainWindow::mousePressEvent(QMouseEvent *event){
    if(rubberBand != NULL){
        rubberBand->hide();
    }
    point = event->pos();
    if(isSelect){
        ui->scrollArea->setCursor(QCursor(Qt::CrossCursor));
        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        rubberBand->setGeometry(QRect(point, QSize()));
        rubberBand->show();
        show();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    pt = event->pos();
    diff = pt-point;
    if(isSelect){
       rubberBand->setGeometry(QRect(point, event->pos()).normalized());
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if(isSelect){
        a = mapToGlobal(point);
        b = event->globalPos();

        /* check all possibility */
        if(a.x() != b.x() && a.y() != b.y() && isSelect) {
               if(a.x() > b.x() && a.y() < b.y() )
               {
                   int y = a.x();
                   a.setX(b.x());
                   b.setX(y);
               }

               if(a.x() > b.x() && a.y() > b.y() )
               {
                   int x = a.x();
                   int y = a.y();
                   a.setX(b.x());
                   a.setY(b.y());
                   b.setX(x);
                   b.setY(y);
               }

               if(a.x() < b.x() && a.y() > b.y() )
               {
                   int y1 = b.y();
                   int y2 = a.y();
                   a.setY(y1);
                   b.setY(y2);
               }

            a = ui->imageLabel->mapFromGlobal(a);
            b = ui->imageLabel->mapFromGlobal(b);
        }
    }
}
