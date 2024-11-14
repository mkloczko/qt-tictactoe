#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <array>

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

protected:
    std::array<SlotState,9> m_slots;
};

#endif // GAMESTATE_H
