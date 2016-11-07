#ifndef PLANETEDITOR_H
#define PLANETEDITOR_H

#include <QWidget>
#include <QColorDialog>
#include "planet.h"

namespace Ui {
class PlanetEditor;
}

class PlanetEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlanetEditor(QWidget *parent = 0);
    ~PlanetEditor();
    void updatePlanetInfo();
    Planet p;
    
private slots:
    void on_pushButton_clicked();
    
    void on_pushButton_3_clicked();
    
    void on_pushButton_2_clicked();
    
private:
    Ui::PlanetEditor *ui;
   
signals:
    void creation_finished(bool success = 1);
};

#endif // PLANETEDITOR_H
