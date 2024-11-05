#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>
#include <array>

class GameState : public QObject
{
    Q_OBJECT
public:
    enum class SlotState {
        Empty,
        Nough,
        Cross,
    }; Q_ENUM(SlotState)

    enum class Initiative {
        Player,
        Opponent,
        Finish
    }; Q_ENUM(Initiative)

    explicit GameState(QObject *parent = nullptr);

    const std::array<SlotState,9> & getSlots() const;
    void setSlotAt(int ix, SlotState state);
    SlotState getSlotAt(int ix) const;

    void restart(Initiative newInitiative);

signals:
    //Will need a signal to show that slots have changed - and/or only specific slots. For drawing purposes.
protected:
    std::array<SlotState,9> m_slots;
    // bool m_gameStarted;
    Initiative m_initiative;
};

#endif // GAMESTATE_H
