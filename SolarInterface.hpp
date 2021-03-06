#ifndef SOLAR_INTERFACE_H
#define SOLAR_INTERFACE_H

#include <QtGui>
#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include <QTextStream>
#include <QLineEdit>

#include "SolarSystem.hpp"

class SolarInterface : public QWidget
{
    Q_OBJECT

    public:

        using coord_t = std::pair< int, int >;

        SolarInterface(QWidget *parent = 0);
        void on_start_load(const char *);
        void timerEvent(QTimerEvent *);
        void paintEvent(QPaintEvent *);
        void clear (QPainter & paint);
        ~SolarInterface();

    private slots:

        void on_setup_b_clicked();
        void on_save_b_clicked();
        void on_load_b_clicked();
        void on_in_text_clicked();
        void on_speed_up_clicked();
        void on_speed_down_clicked();

    private:

        QPushButton * save_b, * load_b, * in_text,
                    * speed_up, * speed_down, * setup_b;
        QTimer * timer;
        QLineEdit * inp;
        SolarSystem my_system;
        double curr_time; // period: 6,3
        double speed;
        QString dir;

        std::vector< coord_t > stars;
};

#endif // SOLAR_INTERFACE_H
