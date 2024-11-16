#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <array>

struct BoardResult {
    Q_GADGET
    QML_NAMED_ELEMENT(boardResult)
    Q_PROPERTY(bool isFinished MEMBER isFinished)
    Q_PROPERTY(int winningColumn MEMBER winningColumn)
    Q_PROPERTY(int winningRow MEMBER winningRow)
    Q_PROPERTY(bool isTopLeftDiagonalWinning MEMBER isTopLeftDiagonalWinning)
    Q_PROPERTY(bool isTopRightDiagonalWinning MEMBER isTopRightDiagonalWinning)
public:
    bool isFinished = false;
    int winningColumn = -1;
    int winningRow = -1;
    bool isTopLeftDiagonalWinning = false;
    bool isTopRightDiagonalWinning = false;
};

class GameState : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum class SlotState {
        Empty,
        Nough,
        Cross,
    }; Q_ENUM(SlotState)

    explicit GameState(QObject *parent = nullptr);

    const std::array<SlotState,9> & getSlots() const;
    void setSlotAt(int ix, SlotState state);
    Q_INVOKABLE SlotState getSlotAt(int ix) const;

    BoardResult checkForEndCondition(int ix);

protected:
    std::array<SlotState,9> m_slots;
};

#endif // GAMESTATE_H
