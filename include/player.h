#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
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

    virtual QFuture<int> play(const BoardState * boardState, int lastMove);
public slots:
    void startTurn(const BoardState * boardState, int lastMove);
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

    virtual QFuture<int> play(const BoardState * boardState, int lastMove) override;
    Q_INVOKABLE void selectSlot(int ix);
protected:
    QMutex m_mutex;
    int m_selection = -1;
};

#endif // PLAYER_H
