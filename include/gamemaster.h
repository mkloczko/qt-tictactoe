#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include <QObject>

class GameState;

class GameMaster : public QObject
{
    Q_OBJECT
public:
    enum class Initiative {
        Player,
        Opponent,
        Finish
    }; Q_ENUM(Initiative)

    explicit GameMaster(QObject *parent = nullptr);

    void restart(Initiative newInitiative);
    bool playAt(int ix);
    Initiative getInitiative();


protected:
    GameState * m_gameState = nullptr;
    Initiative m_initiative;
signals:
};

#endif // GAMEMASTER_H
