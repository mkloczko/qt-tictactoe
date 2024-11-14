#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include "gamestate.h"

class GameMaster : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const GameState* gameState READ getGameState)
    QML_ELEMENT
public:
    enum class Initiative {
        Player,
        Opponent,
        Finish
    }; Q_ENUM(Initiative)

    explicit GameMaster(QObject *parent = nullptr);

    Q_INVOKABLE void restart(Initiative newInitiative);
    Q_INVOKABLE bool playAt(int ix);

    const GameState * getGameState() const {return m_gameState;}
    const Initiative getInitiative() const {return m_initiative;}

protected:
    GameState * m_gameState = nullptr;
    Initiative m_initiative;
signals:
};

#endif // GAMEMASTER_H
