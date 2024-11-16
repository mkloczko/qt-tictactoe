#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include "boardstate.h"
#include "player.h"

class GameMaster : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const BoardState* boardState READ getBoardState)
    QML_ELEMENT
public:
    enum class Initiative {
        Nough,
        Cross,
        Finish
    }; Q_ENUM(Initiative)

    explicit GameMaster(QObject *parent = nullptr);

    Q_INVOKABLE void restart(Initiative newInitiative, bool humanNough, bool humanCross);
    Q_INVOKABLE HumanPlayer * getCurrentHumanPlayer();

    const BoardState * getBoardState() const {return m_boardState;}
    const Initiative getInitiative() const {return m_initiative;}

public slots:
    void noughMove(int ix);
    void crossMove(int ix);

signals:
    void boardUpdated(int ix);
    void finished(BoardResult result);
    void restarted();

protected:
    void performMove(Initiative who, int ix);
    void askNext();

    BoardState * m_boardState = nullptr;
    Initiative m_initiative;
    int m_lastMove = -1;

    Player * m_noughPlayer;
    Player * m_crossPlayer;
};

#endif // GAMEMASTER_H
