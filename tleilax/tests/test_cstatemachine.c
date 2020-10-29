//
// Created by geoco on 27.10.2020.
//
#include <unity.h>
#include <StateMachine.h>
#include <stdint.h>

typedef struct SpaceShip
{
    uint32_t currentSpeed;
};

typedef struct SpaceShipData
{
    uint32_t speed;
};

EVENT_DECLARE(SHIP_SetSpeed, struct SpaceShipData)
EVENT_DECLARE(SHIP_Halt, NoEventData)

enum States
{
    ST_IDLE,
    ST_STOP,
    ST_START,
    ST_CHANGE_SPEED,
    ST_MAX_STATES
};

STATE_DECLARE(Idle, NoEventData);
STATE_DECLARE(Stop, NoEventData);
STATE_DECLARE(Start, struct SpaceShipData);
STATE_DECLARE(ChangeSpeed, struct SpaceShipData);

BEGIN_STATE_MAP(SpaceShip)
    STATE_MAP_ENTRY(ST_Idle)
    STATE_MAP_ENTRY(ST_Stop)
    STATE_MAP_ENTRY(ST_Start)
    STATE_MAP_ENTRY(ST_ChangeSpeed)
END_STATE_MAP(SpaceShip)

EVENT_DEFINE(SHIP_SetSpeed, struct SpaceShipData)
{
    /* Given the SetSpeed event, transition to a new state based on the state defined in the entry */
    BEGIN_TRANSITION_MAP
        TRANSITION_MAP_ENTRY(ST_START) // ST_Idle (SetSpeed) -> ST_START
        TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_Stop (SetSpeed) -> nothing happens
        TRANSITION_MAP_ENTRY(ST_CHANGE_SPEED) // ST_Start (SetSpeed) -> ST_CHANGE_SPEED
        TRANSITION_MAP_ENTRY(ST_CHANGE_SPEED) // ST_ChangeSpeed (SetSpeed) -> ST_CHANGE_SPEED
    END_TRANSITION_MAP(SpaceShip, pEventData)
}

EVENT_DEFINE(SHIP_Halt, NoEventData)
{
    BEGIN_TRANSITION_MAP
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)
        TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)
        TRANSITION_MAP_ENTRY(ST_STOP)
        TRANSITION_MAP_ENTRY(ST_STOP)
    END_TRANSITION_MAP(SpaceShip, pEventData)
}

STATE_DEFINE(Idle, NoEventData)
{
    printf("%s ST_Idle\n", self->name);
}

STATE_DEFINE(Stop, NoEventData)
{
    struct SpaceShip *pInstance = SM_GetInstance(struct SpaceShip);
    pInstance->currentSpeed = 0;
    printf("%s ST_Stop: %d\n", self->name, pInstance->currentSpeed);
    SM_InternalEvent(ST_IDLE, NULL);
}

STATE_DEFINE(Start, struct SpaceShipData)
{
    ASSERT_TRUE(pEventData);
    struct SpaceShip *pInstance = SM_GetInstance(struct SpaceShip);
    pInstance->currentSpeed = pEventData->speed;
    printf("%s ST_Start: %d\n", self->name, pInstance->currentSpeed);

}

STATE_DEFINE(ChangeSpeed, struct SpaceShipData)
{
    ASSERT_TRUE(pEventData);
    struct SpaceShip *pInstance = SM_GetInstance(struct SpaceShip);
    pInstance->currentSpeed = pEventData->speed;
    printf("%s ST_ChangeSpeed: %d\n", self->name, pInstance->currentSpeed);
}
void setUp(void) {}
void tearDown(void) {}

static void test_Should_Start_From_Idle_State()
{
    static struct SpaceShip spaceShip;
    SM_DEFINE(SpaceShipSM, &spaceShip);
    struct SpaceShipData *spaceShipData;
    spaceShipData = SM_XAlloc(sizeof(struct SpaceShipData));
    spaceShipData->speed = 100;

    TEST_ASSERT_EQUAL(ST_IDLE, SpaceShipSMObj.currentState);
    TEST_ASSERT_EQUAL(spaceShip.currentSpeed, 0);

    SM_Event(SpaceShipSM, SHIP_SetSpeed, spaceShipData);

    TEST_ASSERT_EQUAL(ST_START, SpaceShipSMObj.currentState);
    TEST_ASSERT_EQUAL(spaceShip.currentSpeed, 100);

    SM_XFree(spaceShipData);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_Should_Start_From_Idle_State);
//    RUN_TEST(test_Should_Load_Star_Names);
    return UNITY_END();
}