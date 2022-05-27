#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<cmath>

int mas[9][9];      //для вычислений пути
int array[9][9];    //отрисовки линий черным цветом
int count;          //Количество вершин
int con = 7;        //con - вспомогательная переменная(крашфикс)
int inc;            //inc - инкремент - переменная для рисования
int level=0;        //level - текущий этап
int row_min[9];
int column_min[9];  //Массивы минимальных элементов в каждой строке и в каждом столбце
int min_way[9][2];
int mid_arg;
int mid_func;
int way[9];                 //Наикратчайщий путь
int blacklin[100];          //двусторонние дороги
int d = 1;                  //номер вершины, которую нужно удалить
int lim = 12,lim2 = 12;     //вершины,которые удалены
int ab = 1;                 //количество удаленных вершин
double grad = 360;          //отрисовкf окружности

class Coord                 //Координаты для построения ребер графа и для окружностей
{
public:
    int x,y;
};

Coord *coord;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setVisible(false);
    ui->del->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_create_clicked()
{
    ui->label_2->hide();
  count = con;
  ui->build->show();
  HideTable();
  ShowTable();
  coord = new Coord[count];
  mid_arg = 250;
  mid_func = 180;
  level = 1;
  ui->clear->show();
  this->repaint();
}
void MainWindow::on_build_clicked()                                                 //построение путей между вершинами
{
    ui->del->show();
    ui->spinBox->show();

  count = con;
  QString linA,linB;
  for(int i = 0; i < count; i++) {
      for(int j = 0; j < count; j++) {
          if(i != j) {
              linA = "road" + QString::number(i+1) + QString::number(j+1);
              linB = this->findChild<QLineEdit*>(linA)->text();
              array[i][j] = linB.toInt();
              if(!linB.length())
                  linB = QString::number(INT32_MAX);
              mas[i][j] = linB.toInt();
          }
          if(i == j) {
              mas[i][j] = -1;
              array[i][j] = -1;
          }
          if(i == lim-1 || j == lim-1 || i == lim2-1 || j == lim2-1) mas[i][j] = -1;
      }
  }
  level = 2;
  this->repaint();
}

void MainWindow::on_calc_clicked()
{
  QString msg1,msg2;
  QString linA,linB;

  for(int i = 0; i < count; i++) {
      find_min();                                                                   //Функция поиска минимального элемента
      find_null(i);                                                                 //Функция поиска элементов для оптимального пути
  }

  for(int i = 0; i < count; i++) {
      for(int j = 0; j < count; j++) {
          if(i != j) {
              linA="road" + QString::number(i+1) + QString::number(j+1);
              linB = this->findChild<QLineEdit*>(linA)->text();
              if(!linB.length())
                  linB=QString::number(INT32_MAX);
              mas[i][j] = linB.toInt();
          }
          else
              mas[i][j] = -1;
      }
  }
  msg2 += "\nОптимальный путь: " + QString::number(min_way[0][0]+1) + " -> " + QString::number(min_way[0][1]+1);
  int point = min_way[0][1], summ = mas[min_way[0][0]][min_way[0][1]];

  way[0] = min_way[0][0];
  way[1] = min_way[0][1];

  int z = 1;
  while(z < count) {
      for(int i = 1; i < count; i++) {
          if(min_way[i][0] == point && min_way[i][0] != min_way[i][1]) {
              way[z+1] = point = min_way[i][1];
              msg2 += " -> " + QString::number(min_way[i][1]+1);
              summ += mas[min_way[i][0]][min_way[i][1]];
              min_way[i][0] = -1;
              min_way[i][1] = -1;
              break;
          }
      }
      z++;
  }
  level = 3;
  this->repaint();
  msg1 += "\nПродолжительность: " + QString::number(summ);
  ui->label_2->show();
  ui->label_2->setText(msg1 + msg2);
}

void MainWindow::find_min() {                                                          //Функция поиска минимального элемента для оптимального пути
    for(int i=0; i < count; i++) {
        row_min[i] = INT32_MAX;
        column_min[i] = INT32_MAX;
    }
    for(int i=0; i<count; i++) {
        for(int j=0; j<count; j++)
            if(i != j && mas[i][j]>-1 && mas[i][j] < row_min[i])
                row_min[i] = mas[i][j];
        for(int j=0; j<count; j++)
            if(i != j && row_min[i] > 0 && row_min[i] < 1000)
                mas[i][j]  -= row_min[i];
    }
    for(int j=0; j< count; j++) {
        for(int i=0; i< count; i++)
            if(i != j && mas[i][j] > -1 && mas[i][j] < column_min[j])
                column_min[j] = mas[i][j];
        for(int i=0; i<count; i++)
            if(i != j && column_min[j] > 0 && column_min[j] < 1000)
                mas[i][j] -= column_min[j];
    }
}

void MainWindow::find_null(int num) {                                                    //Функция поиска элементов для оптимального пути
    int min_in_row = INT32_MAX;
    int min_in_column = INT32_MAX;
    int max_rank_null = 0;
    int max_i = 100, max_j = 100;

    for(int i=0; i<count; i++) {
        for(int j=0; j<count; j++) {
            if(mas[i][j] == 0 && i!=j) {
                for(int k=0; k<count; k++) {
                    if(mas[k][j] > -1 && mas[k][j] < min_in_column && k!=j && k!=i)
                        min_in_column = mas[k][j];
                    if(mas[i][k] > -1 && mas[i][k] < min_in_row && i!=k && k!=j)
                        min_in_row = mas[i][k];
                }
                if(min_in_row > 1000)
                    min_in_row = 0;
                if(min_in_column > 1000)
                    min_in_column = 0;
                if(min_in_row > -1 && min_in_column > -1 && i!=j && max_rank_null <= (min_in_row + min_in_column)) {
                    max_rank_null = min_in_row + min_in_column;
                    max_i = i;
                    max_j = j;
                }
            }
        }
    }
    min_way[num][0] = max_i;
    min_way[num][1] = max_j;

    for(int z=0; z<count; z++) {
        mas[max_i][z] = -1;
        mas[z][max_j] = -1;
    }
    mas[max_j][max_i] = -1;
};


void MainWindow::HideTable() //Скрыть таблицу
{
    QString str;
    for(int i = 1; i < 8; i++) {
        str="column" + QString::number(i);
        this->findChild<QWidget*>(str)->hide();

        str="row" + QString::number(i);
        this->findChild<QWidget*>(str)->hide();

        str="minus" + QString::number(i);
        this->findChild<QWidget*>(str)->hide();

        for(int j = i+1; j < 8; j++) {
            str="road" + QString::number(i) + QString::number(j);
            this->findChild<QWidget*>(str)->hide();
        }
        for(int j = 1; j < i; j++) {
            str="road" + QString::number(i) + QString::number(j);
            this->findChild<QWidget*>(str)->hide();
        }
    }
}
void MainWindow::ShowTable() {//показать таблицу
  QString str;
  for(int i = 1; i < count+1; i++) {
      str="column" + QString::number(i);
      this->findChild<QWidget*>(str)->show();
  }
  for(int i = 1; i < count+1; i++) {
      str="row" + QString::number(i);
      this->findChild<QWidget*>(str)->show();

      str="minus" + QString::number(i);
      this->findChild<QWidget*>(str)->show();

      for(int j=i+1; j < count+1; j++) {
          str="road" + QString::number(i) + QString::number(j);
          this->findChild<QWidget*>(str)->show();
      }
      for(int j = 1; j < i; j++) {
          str="road" + QString::number(i) + QString::number(j);
          this->findChild<QWidget*>(str)->show();
      }
  }
}

void MainWindow::paintEvent(QPaintEvent *) { //отрисовка графа
    if(level) {

        grad /= count;
        inc = grad;
        grad = 0;
        QPainter paint(this);
        QPen pen;
        QBrush brush;
        QString str;
        QFont font;

        paint.begin(this);
        paint.setPen(pen);
        paint.setFont(font);
        paint.setBrush(brush);

        pen.setColor(Qt::red);
        pen.setWidth(7);
        pen.setStyle(Qt::SolidLine);
        font.setPointSize(40);

        brush.setStyle(Qt::Dense3Pattern);
        brush.setColor(Qt::red);

        for(int i=0; i<count; i++) {
            str=QString::number(i+1);
            coord[i].x = mid_arg+(100*cos(grad/57));
            coord[i].y = mid_func+(100*-sin(grad/57));
            paint.setPen(QPen(Qt::black,3,Qt::SolidLine,Qt::FlatCap));
            paint.setBrush(QBrush(Qt::white, Qt::SolidPattern));
            paint.setFont(QFont("Arial",12));
            grad += inc;
            if(i != lim-1 && i!= lim2-1) {
            paint.drawEllipse(coord[i].x,coord[i].y,50,50);
            paint.setPen(QPen(Qt::black,2,Qt::SolidLine,Qt::FlatCap));
            paint.drawText(coord[i].x+0,coord[i].y+0,str);
        }
        }
        if(level > 1) {
            int t = 0;
            pen.setStyle(Qt::DashLine);
             pen.setColor(Qt::black);
            pen.setWidth(3);
            paint.setPen(pen);
            for(int i = 0; i < count; i++) {
                for(int j=0; j< count; j++) {
                    if((mas[i][j]<1000 && mas[i][j] > 0) && (i != lim-1 && j != lim-1 && i!= lim2-1 && j!= lim2-1)) {
                        paint.drawLine(coord[i].x+25,coord[i].y+25,coord[j].x+25,coord[j].y+25);
                    }
                }
            }
            pen.setColor(Qt::black);
            pen.setStyle(Qt::SolidLine);
             pen.setWidth(4);
             paint.setPen(pen);
            for(int i = 0; i < count; i++) {
                for(int j = i+1; j < count; j++) {
                    if(array[i][j] > 0) {
                        blacklin[t] = j * 10 + i;
                        t++;
                    }
                }
            }
             for(int i = 0; i < count; i++) {
                 for(int j = 0; j < i; j++) {
                     for(int g = 0; g < t; g++) {
                         if((i*10+j) == blacklin[g] && array[i][j] > 0 && i!=lim-1 && j!=lim-1 && i!=lim2-1 && j!=lim2-1) {
                             paint.drawLine(coord[i].x+25,coord[i].y+25,coord[j].x+25,coord[j].y+25);
                         }
                     }
                 }
             }
        }

        if(level == 3) {
            int reg = 0;
            if(lim < 8) reg++;
            if(lim2 < 8) reg++;
            pen.setStyle(Qt::SolidLine);
            pen.setWidth(7);
            pen.setColor(Qt::red);
            paint.setPen(pen);
            for(int i=1; i<count+1-reg; i++)
                paint.drawLine(coord[way[i-1]].x+25,coord[way[i-1]].y+25,coord[way[i]].x+25,coord[way[i]].y+25);
        }
        paint.end();
    }
}

void MainWindow::on_Spinbox_valueChanged(int arg1)
{
   con = arg1;
     ui->spinBox->setMaximum(arg1);
}

void MainWindow::on_clear_clicked()
{
    QString str;
    for(int i = 1; i < 8; i++) {
    for(int j = i+1; j < 8; j++) {
        str="road" + QString::number(i) + QString::number(j);
        this->findChild<QLineEdit*>(str)->setText("1");
         this->findChild<QLineEdit*>(str)->setStyleSheet("background-color: rgb(255,255,255)");
    }
    for(int j = 1; j < i; j++) {
        str="road" + QString::number(i) + QString::number(j);
        this->findChild<QLineEdit*>(str)->setText("1");
        this->findChild<QLineEdit*>(str)->setStyleSheet("background-color: rgb(255,255,255)");
    }
}
    lim = 12;
    lim2 = 12;
    ab = 1;
    ui->create->click();
}

void MainWindow::on_del_clicked() //удалить вершину
{
    if(ab == 2) lim2 = lim;
    QString str;
    lim = d;
    for(int i = 1; i < count+1; i++) {
    for(int j = i+1; j < count+1; j++) {
        if(i == lim || j == lim) {
        str="road" + QString::number(i) + QString::number(j);
        this->findChild<QLineEdit*>(str)->setText("");
        this->findChild<QLineEdit*>(str)->setStyleSheet("background-color: rgb(141,141,141);");
    }
    }
    for(int j = 1; j < i; j++) {
        if(i == lim || j == lim) {
        str="road" + QString::number(i) + QString::number(j);
        this->findChild<QLineEdit*>(str)->setText("");
        this->findChild<QLineEdit*>(str)->setStyleSheet("background-color: rgb(141,141,141);");
    }
    }
}
    this->repaint();
    ui->build->click();
    ab++;
}
void MainWindow::on_spinBox_valueChanged(int arg1)
{
    d = arg1;
}

