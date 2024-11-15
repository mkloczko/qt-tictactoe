#include "player.h"

#include <QtConcurrent/QtConcurrent>
#include <QRandomGenerator>

namespace {
    template <typename T>
    QFuture<T> rawFuture(const T & value) {
        QFutureInterface<T> futureInterface;
        futureInterface.reportFinished(&value);
        return QFuture<T>(&futureInterface);
    }
}

Player::Player(Team team, QObject *parent)
    : QObject{parent}, m_team(team)
{}

Player::~Player()
{
    m_worker.waitForFinished();
}

QFuture<int> Player::play(const GameState * gameState, int lastMove)
{
    return QtConcurrent::run([this]() -> int {
        QThread::msleep(QRandomGenerator::global()->bounded(150,600));
        return QRandomGenerator::global()->bounded(0,9);
    });
}

void Player::startTurn(const GameState * gameState, int lastMove)
{
    m_worker = play(gameState, lastMove);
    m_worker.then([this](int result) {
        emit endTurn(result);
    }).onFailed([this] {
        emit endTurn(-1);
    });
}

HumanPlayer::~HumanPlayer()
{
    m_selection = -1;
    m_mutex.tryLock();
    m_mutex.unlock();
}

QFuture<int> HumanPlayer::play(const GameState * gameState, int lastMove)
{
    m_mutex.lock();
    return QtConcurrent::run([this]() -> int {
        m_mutex.lock();
        int ret = m_selection;
        m_mutex.unlock();
        return ret;
    });
}

void HumanPlayer::selectSlot(int ix)
{
    m_mutex.unlock();
    m_selection = ix;
}
