#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QLabel>
#include <QRubberBand>
#include <QScrollArea>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool loadFile(const QString &);
    ~MainWindow();

private slots:
    void open();

    void save_as();

    void zoom_in();

    void zoom_out();

    void rotate();

    void undo();

    void redo();

    void reset();

    void about();

    void select();

    void crop();

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

private:
    struct ImageStruct {
        QImage srcImg;
        QImage editedImg;
        int rotAngle;
    };

    Ui::MainWindow *ui;
    QImage image;
    QImage originImage;
    QImage sourceImage;
    QScrollArea *scrollArea;
    int rotationAngle;
    double scaleFactor;

    QAction *openAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *rotateAct;
    QAction *aboutAct;
    QAction *resetAct;
    QAction *selectAct;
    QAction *cropAct;

    QStack<ImageStruct> *originStack;
    QStack<ImageStruct> *redoStack;

    QRubberBand *rubberBand;
    bool isSelect;
    QPoint point;
    QPoint a;
    QPoint b;

    void createActions();
    void createMenus();
    void enableActions();
    void disableActions();
    bool saveFile(const QString &);
    void setImage(const QImage &newImage);
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode);
};

#endif // MAINWINDOW_H
