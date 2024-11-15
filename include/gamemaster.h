#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include "gamestate.h"
#include "player.h"

class GameMaster : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const GameState* gameState READ getGameState)
    QML_ELEMENT
public:
    enum class Initiative {
        Nough,
        Cross,
        Finish
    }; Q_ENUM(Initiative)

    explicit GameMaster(QObject *parent = nullptr);

    Q_INVOKABLE void restart(Initiative newInitiative);
    Q_INVOKABLE HumanPlayer * getCurrentHumanPlayer();

    const GameState * getGameState() const {return m_gameState;}
    const Initiative getInitiative() const {return m_initiative;}

public slots:
    void noughMove(int ix);
    void crossMove(int ix);

signals:
    void boardUpdated();

protected:
    void performMove(Initiative who, int ix);
    void askNext();
    bool checkForEndCondition(int ix);

    GameState * m_gameState = nullptr;
    Initiative m_initiative;
    int m_lastMove = -1;

    Player * m_noughPlayer;
    Player * m_crossPlayer;
};

#endif // GAMEMASTER_H
