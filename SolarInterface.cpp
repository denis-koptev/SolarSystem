#include "SolarInterface.hpp"
#include <string>
#include <QFileDialog>

#define STARS_NUM 500

static SolarInterface::coord_t get_random_coordinates(int x_start, int x_end,
                                                      int y_start, int y_end)
{
    int x, y;
    x = (rand() % (x_end-x_start+1) + x_start);
    y = (rand() % (y_end-y_start+1) + y_start);

    return SolarInterface::coord_t(x, y);
}

SolarInterface::SolarInterface(QWidget *parent)
    : QWidget(parent)
{
    for (int i = 0; i < STARS_NUM; ++i) {
        coord_t coord = get_random_coordinates(100, 1270, 10, 710);
        stars.push_back(coord);
    }

    // ui->setUi(this);
    setWindowTitle("Solar System");
    setMinimumSize(1280, 720);
    setMaximumSize(1280, 720);

    setup_b = new QPushButton("SETUP", this);
    setup_b->setGeometry(5, 20, 90, 30);

    save_b = new QPushButton("SAVE", this);
    save_b->setGeometry(5, 70, 90, 30);

    load_b = new QPushButton("LOAD", this);
    load_b->setGeometry(5, 120, 90, 30);

    in_text = new QPushButton("PRINT", this);
    in_text->setGeometry(5, 170, 90, 30);

    speed_up = new QPushButton("SPEED +", this);
    speed_up->setGeometry(5, 220, 90, 30);

    speed_down = new QPushButton("SPEED -", this);
    speed_down->setGeometry(5, 270, 90, 30);

    QMetaObject::connectSlotsByName(save_b);
    QObject::connect(setup_b, SIGNAL(clicked()), SLOT(on_setup_b_clicked()));
    QObject::connect(save_b, SIGNAL(clicked()), SLOT(on_save_b_clicked()));
    QObject::connect(load_b, SIGNAL(clicked()), SLOT(on_load_b_clicked()));
    QObject::connect(in_text, SIGNAL(clicked()), SLOT(on_in_text_clicked()));
    QObject::connect(speed_up, SIGNAL(clicked()), SLOT(on_speed_up_clicked()));
    QObject::connect(speed_down, SIGNAL(clicked()), SLOT(on_speed_down_clicked()));

    curr_time = 0;
    speed =  0.01;
    startTimer(20);

    /* Initialize working directory as current directory */
    dir = "./";
}

void SolarInterface::timerEvent(QTimerEvent *)
{
    //static long long int iterations = 0;
    //iterations++;

    /*if (iterations >= 100) {
        this->close();
    }*/

    update();
    // recountTime();
}

void SolarInterface::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    clear(paint);
    paint.fillRect(100, 10, 1170, 700, QBrush(Qt::black));
    paint.setBrush(QBrush(Qt::white));

    if (my_system.getSize() == 0) return;

    try {
        my_system.planetsMovement(curr_time);
    }
    catch (const std::exception & ex) {
        QString error(ex.what());
        paint.setPen(Qt::red);
        paint.drawText(110, 700, error.toStdString().c_str());
        paint.setPen(Qt::black);
        curr_time -= speed;
    }

    std::string info = "";

    std::vector<Planet> planets = my_system.getPlanets();
    Star sun = my_system.getStar();

    for (int i = 0; i < STARS_NUM; ++i) {
        int star_x = stars[i].first;
        int star_y = stars[i].second;
        paint.drawEllipse(star_x, star_y, 2, 2);
    }

    paint.setBrush(QBrush(Qt::yellow));
    paint.drawEllipse(220 + sun.getPosition().getX() - sun.getRad(),
            10 + sun.getPosition().getY() - sun.getRad(),
            2 * sun.getRad(), 2 * sun.getRad());

    paint.setPen(Qt::white);
    info += sun.getName() + ": " + doubleToString(sun.getPosition().getX()) +
        ", " + doubleToString(sun.getPosition().getY());
    paint.drawText(110, 30, info.c_str());
    paint.setPen(Qt::black);

    for (std::vector<Planet>::iterator i = planets.begin(); i != planets.end(); ++i) {
        info = i->getName() + ": " + doubleToString(i->getPosition().getX()) +
            ", " + doubleToString(i->getPosition().getY());
        paint.setPen(Qt::white);
        paint.drawText(110, 45 + 15 * std::distance(planets.begin(), i), info.c_str());
        paint.setPen(Qt::black);

        if (i->getColor() == "blue") {
            paint.setBrush(QBrush(Qt::blue));
        }
        else if (i->getColor() == "red") {
            paint.setBrush(QBrush(Qt::red));
        }
        else if (i->getColor() == "green") {
            paint.setBrush(QBrush(Qt::green));
        }
        else if (i->getColor() == "gray") {
            paint.setBrush(QBrush(Qt::gray));
        }
        else if (i->getColor() == "magenta") {
            paint.setBrush(QBrush(Qt::magenta));
        }
        else if (i->getColor() == "orange") {
            QColor c("orange");
            paint.setBrush(QBrush(c));
        }
        else {
            paint.setBrush(QBrush(Qt::white));
        }

        paint.drawEllipse(220 + i->getPosition().getX() - i->getRad(),
                10 + i->getPosition().getY() - i->getRad(),
                2 * i->getRad(), 2 * i->getRad());
    }

    if (curr_time > 6.28) {
        curr_time = 0;
    }

    curr_time += speed;
}

void SolarInterface::clear(QPainter & paint)
{
    paint.eraseRect(100, 10, 1170, 700);
}

SolarInterface::~SolarInterface()
{
    delete load_b;
    delete save_b;
    delete setup_b;
    delete in_text;
    delete speed_up;
    delete speed_down;
}

void SolarInterface::on_save_b_clicked()
{
    std::cout << "[SAVE] Saving state to condition.log" << std::endl;
    try {
        my_system.save((dir.toStdString() + "/condition.log").c_str());
    }
    catch (const std::exception & ex) {
        std::cout << ex.what() << std::endl;
    };
}

void SolarInterface::on_load_b_clicked()
{
    std::cout << "[LOAD] Loading state from condition.log" << std::endl;
    try {
        my_system.input((dir.toStdString() + "/condition.log").c_str());
    }
    catch (const std::exception & ex) {
        std::cout << ex.what() << std::endl;
    };

    curr_time = 0;
}

void SolarInterface::on_in_text_clicked()
{
    std::cout << "[OUTPUT] Printing state to out.txt" << std::endl;
    try {
        my_system.output((dir.toStdString() + "/out.txt").c_str(), true);
    }
    catch (const std::exception & ex) {
        std::cout << ex.what() << std::endl;
    };
}

void SolarInterface::on_speed_up_clicked()
{
    if (speed < 0.1) {
       speed += 0.001;
    }
    std::cout << "[SPEED UP] New speed: " << speed << std::endl;
}

void SolarInterface::on_speed_down_clicked()
{
    if (speed >= 0.002)
    {
        speed -= 0.001;
    }
    std::cout << "[SPEED DOWN] New speed: " << speed << std::endl;
}

void SolarInterface::on_setup_b_clicked()
{
    std::cout << "[SETUP] User is loading model" << std::endl;

    QString filename = QFileDialog::getOpenFileName();
    dir = filename;
    dir.truncate(dir.lastIndexOf('/'));

    try {
        my_system.input(filename.toStdString().c_str());
        my_system.output((dir.toStdString() + "/out.txt").c_str(), false);
    }
    catch (const std::exception & ex) {
        std::cout << ex.what() << std::endl;
    };

    curr_time = 0;
}

void SolarInterface::on_start_load(const char * filename)
{
    if (filename == NULL) {
        throw std::invalid_argument("No filename specified");
    }

    std::cout << "[ON START LOAD] Loading from argument: " << filename << std::endl;

    try {
        my_system.input(filename);
    }
    catch (const std::exception & ex) {
        std::cerr << ex.what() << std::endl;
    }

    curr_time = 0;
}

