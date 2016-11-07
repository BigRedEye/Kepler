#include "planeteditor.h"
#include "ui_planeteditor.h"

PlanetEditor::PlanetEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlanetEditor)
{
    ui->setupUi(this);
}

PlanetEditor::~PlanetEditor()
{
    delete ui;
}

void PlanetEditor::on_pushButton_clicked()
{
    p.x = ui->px->value();
    p.y = ui->py->value();
    p.vx = ui->pvx->value();
    p.vy = ui->pvy->value();
    
    p.r = ui->pr->value();
    p.m = ui->pm->value();
    
    p.color = QColor(ui->rgbr->value(),
                     ui->rgbg->value(),
                     ui->rgbb->value());
    p.name = ui->lineEdit->text();
    p.trackLength = 0;
        
    emit creation_finished(1);
}

void PlanetEditor::on_pushButton_3_clicked()
{
    emit creation_finished(0);
}

void PlanetEditor::updatePlanetInfo()
{
    ui->px->setValue(p.x);
    ui->py->setValue(p.y);
    ui->pvx->setValue(p.vx);
    ui->pvy->setValue(p.vy);
    ui->pr->setValue(p.r);
    ui->pm->setValue(p.m);
    
    ui->rgbr->setValue(p.color.red());
    ui->rgbg->setValue(p.color.green());
    ui->rgbb->setValue(p.color.blue());
    
    ui->lineEdit->setText(p.name);
    ui->pushButton->setAutoDefault(1);
}

void PlanetEditor::on_pushButton_2_clicked()
{
    p.color = QColorDialog::getColor();
    updatePlanetInfo();
}
