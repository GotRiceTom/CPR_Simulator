#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QDebug>
GameWindow::GameWindow(QWidget *parent, CPR_Model *model) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    gameState = new GameState();

    // listens from the view
    QObject::connect(ui->cprAction, &QPushButton::clicked,
                     this, &GameWindow::on_cprAction_clicked);

    QObject::connect(ui->breathAction, &QPushButton::clicked,
                     this, &GameWindow::on_breathAction_clicked);

    QObject::connect(ui->checkResponseAction, &QPushButton::clicked,
                     this, &GameWindow::on_checkResponseAction_clicked);

    QObject::connect(ui->applyPadsAction, &QPushButton::clicked,
                     this, &GameWindow::on_applyPadsAction_clicked);

    //QObject::connect(ui->checkBreathAndPulseButton, &QPushButton::clicked,
                     //this, &GameWindow::on_checkBreathAction_clicked);

    // talks to the model
    QObject::connect(this, &GameWindow::action,
                     model, &CPR_Model::actionPerformed);
    QObject::connect(ui->normalPlayButton, &QPushButton::clicked,
                     model, &CPR_Model::newGame);
    QObject::connect(ui->normalPlayButton, &QPushButton::clicked,
                     [=]() {ui->stackedWidget->setCurrentIndex(1);});


    // Listens from the model
    QObject::connect(model, &CPR_Model::changeStatusBoxSignal,
                     this, &GameWindow::SetStatusBox);

    QObject::connect(model, &CPR_Model::changeTutorialBoxSignal,
                     this, &GameWindow::SetTutorialBox);

    neckTopLeft = QPoint(444,264);
    neckBottomRight = QPoint(527,296);


    ui->stackedWidget->setCurrentIndex(0);

   // QPixmap pix = QPixmap(":/images/Untitled.png");
    //QCursor c = QCursor(pix,-1,-1);
   // setCursor(c);
}

GameWindow::~GameWindow()
{
    delete ui;
}


void GameWindow::mousePressEvent(QMouseEvent *event)
{

    //  if the currentState of the game is checkForPulse

    qDebug() << "x: " << event->x() << "y: " << event->y();

    if(currentState ==gameState->CHECK_PULSE_AND_BREATHING
            && (event->x() <= neckBottomRight.x() && event->x() >= neckTopLeft.x())
            && (event->y() <= neckBottomRight.y() && event->y() >= neckTopLeft.y()))
    {
        qDebug() << "You clicked on neck";

        emit action(gameState->CHECK_PULSE_AND_BREATHING);

       // QPixmap pix = QPixmap(":image/checkBreathAndPulse.png").scaled(32,32,Qt::KeepAspectRatio);
       // QCursor cursor = QCursor(pix);
        setCursor(Qt::ArrowCursor);

    }else
    {
        qDebug() << "You can't check pulse here";
    }

}

void GameWindow::mouseReleaseEvent(QMouseEvent *event)
{

}

void GameWindow::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "x: " << event->x() << "y: " << event->y();
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{

}

void GameWindow::keyReleaseEvent(QKeyEvent *event)
{

}

void GameWindow::on_callAction_clicked()
{
    emit action(gameState->CALL_FOR_911_AND_AED);
}

void GameWindow::on_cprAction_clicked()
{
    emit action(gameState->GIVE_COMPRESSION);
}

void GameWindow::on_breathAction_clicked()
{
    emit action(gameState->GIVE_BREATH);
}

void GameWindow::on_checkResponseAction_clicked()
{
    emit action(gameState->CHECK_RESPONSIVENESS);
}

void GameWindow::on_checkBreathAction_clicked()
{
    emit action(gameState->CHECK_PULSE_AND_BREATHING);
}

void GameWindow::on_applyPadsAction_clicked()
{
    emit action(gameState->APPLY_PADS);
}

void GameWindow::on_checkBreathAndPulseButton_clicked()
{
   // emit action(gameState->CHECK_PULSE_AND_BREATHING);

    currentState = gameState->CHECK_PULSE_AND_BREATHING;

    QPixmap pix = QPixmap(":images/checkBreathAndPulse.png");
    QPixmap p2 = pix.scaled(32,32,Qt::KeepAspectRatio);
    QCursor cursor = QCursor(p2);
    setCursor(cursor);
}

void GameWindow::SetStatusBox(string status)
{
    ui->scenarioText->setText("Current Scenario: " + QString::fromStdString(status));
}

void GameWindow::SetTutorialBox(string message)
{
    ui->hintText->setText("\nHint: " + QString::fromStdString(message));
}


