#include <QCoreApplication>
#include <QStateMachine>
#include <QState>
#include <QFinalState>
#include <QTimer>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Create states
    QState *state1 = new QState();
    QState *state2 = new QState();
    QFinalState *finalState = new QFinalState();

    // Signal on entering states
    QObject::connect(state1, &QState::entered, []() {
        qDebug() << "Entered State 1";
    });
    QObject::connect(state2, &QState::entered, []() {
        qDebug() << "Entered State 2";
    });

    // Setup state machine
    QStateMachine machine;
    machine.addState(state1);
    machine.addState(state2);
    machine.addState(finalState);
    machine.setInitialState(state1);

    // Transition from state1 to state2 after 2 seconds
    QTimer *timer1 = new QTimer();
    timer1->setSingleShot(true);
    // Connect timer timeout to change state
    QObject::connect(timer1, &QTimer::timeout, [&]() {
        if (machine.configuration().contains(state1))
            state1->machine()->postEvent(new QEvent(QEvent::Type(QEvent::User + 1))); 
    });
    // Or simply use a transition with timer
    state1->addTransition(timer1, &QTimer::timeout, state2);

    // Transition from state2 to final after 3 seconds
    QTimer *timer2 = new QTimer();
    timer2->setSingleShot(true);
    state2->addTransition(timer2, &QTimer::timeout, finalState);
    QObject::connect(&machine, &QStateMachine::started, [&]() {
        timer1->start(2000);
        timer2->start(3000);
    });

    // When the machine finishes, exit app
    QObject::connect(&machine, &QStateMachine::finished, &app, &QCoreApplication::quit);

    // Start machine
    machine.start();

    return app.exec();
}
