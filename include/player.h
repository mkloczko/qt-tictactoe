#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QRandomGenerator>
#include <QFuture>
#include <QMutex>
#include "boardstate.h"

class Player : public QObject
{
    Q_OBJECT
public:
    enum class Team {
        Noughs,
        Crosses
    }; Q_ENUM(Team)

    explicit Player(Team team, QObject *parent = nullptr);
    virtual ~Player();

    virtual QFuture<int> play(const BoardState *boardState) = 0;
public slots:
    void startTurn(const BoardState *boardState);
signals:
    void endTurn(int move);
protected:
    QFuture<int> m_worker;
    Team m_team;
};

class HumanPlayer : public Player
{
    Q_OBJECT
public:
    explicit HumanPlayer(Team team, QObject *parent = nullptr) : Player(team,parent) {}
    virtual ~HumanPlayer();

    virtual QFuture<int> play(const BoardState *boardState) override;
    Q_INVOKABLE void selectSlot(int ix);
protected:
    QMutex m_mutex;
    int m_selection = -1;
};

class ComputerPlayer : public Player
{
    Q_OBJECT
public:
    explicit ComputerPlayer(Team team, QObject *parent = nullptr);

    virtual QFuture<int> play(const BoardState *boardState) override;
protected:
    QFuture<int> sendMove(int move);

    QRandomGenerator m_random;
};

#endif // PLAYER_H
