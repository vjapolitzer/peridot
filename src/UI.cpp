#include "UI.hpp"

bool sendStateRequest(DE_MachineState_t reqState)
{
    char cmd[7];
    sprintf(cmd, "<B>%02X\n",reqState);
    // Logger.info.printf("%s",cmd);
    // return true;
    return de1Serial.write((const uint8_t *)cmd, 7);
}

/******************
 * BTN0 Callbacks *
 ******************/
void onCountBTN0()
{
    gauge.indicateL((uint8_t)0b111);
}

void onPressBTN0()
{
    Logger.info.println("BTN0 pressed");
    switch (myDE1.getState())
    {
    case Sleep:
        Logger.info.println("Cur:Sleep|Req:Idle");
        sendStateRequest(Idle);
        break;
    case Idle:
    case SchedIdle:
        if (myDE1.getSubstate() == NoState)
        {
            Logger.info.println("Curr:Idle|Req:Espresso");
            sendStateRequest(Espresso);
        }
        break;
    case Espresso:
        Logger.info.println("Curr:Espresso|Req:SkipToNext");
        sendStateRequest(SkipToNext);
        break;
    case Steam:
        Logger.info.println("Curr:Steam|Req:SkipToNext");
        sendStateRequest(SkipToNext);
        break;
    case HotWater:
        Logger.info.println("Curr:HotWater|Req:Idle");
        sendStateRequest(Idle);
        break;
    case HotWaterRinse:
        Logger.info.println("Curr:HotWaterRinse|Req:Idle");
        sendStateRequest(Idle);
        break;
    case SteamRinse:
        Logger.info.println("Curr:SteamRinse|Req:Idle");
        sendStateRequest(Idle);
        break;
    case AirPurge:
        Logger.info.println("Curr:AirPurge|Req:Idle");
        sendStateRequest(Idle);
        break;
    default:
        Logger.info.println("Curr:UnhandledState|Req:NONE");
        break;
    }
}

void onDoublePressBTN0()
{
    Logger.info.println("BTN0 double pressed");
    switch (myDE1.getState())
    {
    case Sleep:
        Logger.info.println("Cur:Sleep|Req:Idle");
        sendStateRequest(Idle);
        break;
    // case Idle:
    //     Logger.info.println("Curr:Idle|Req:SteamRinse");
    //     sendStateRequest(SteamRinse);
    //     break;
    default:
        Logger.info.println("Curr:UnhandledState|Req:NONE");
        break;
    }
}

void onLongPressBTN0()
{
    Logger.info.println("BTN0 Long pressed");
    gauge.indicateL((uint8_t)0b111);
    switch (myDE1.getState())
    {
    case Sleep:
        Logger.info.println("Cur:Sleep|Req:Idle");
        sendStateRequest(Idle);
        break;
    case Idle:
    case SchedIdle:
        if (myDE1.getSubstate() == NoState)
        {
            Logger.info.println("Curr:Idle|Req:HotWaterRinse");
            sendStateRequest(HotWaterRinse);
        }
        break;
    case Espresso:
        Logger.info.println("Curr:Espresso|Req:Idle");
        sendStateRequest(Idle);
        break;
    case Steam:
        Logger.info.println("Curr:Steam|Req:Idle");
        sendStateRequest(Idle);
        break;
    case HotWater:
        Logger.info.println("Curr:HotWater|Req:Idle");
        sendStateRequest(Idle);
        break;
    case HotWaterRinse:
        Logger.info.println("Curr:HotWaterRinse|Req:Idle");
        sendStateRequest(Idle);
        break;
    case SteamRinse:
        Logger.info.println("Curr:SteamRinse|Req:Idle");
        sendStateRequest(Idle);
        break;
    case AirPurge:
        Logger.info.println("Curr:AirPurge|Req:Idle");
        sendStateRequest(Idle);
        break;
    default:
        Logger.info.println("Curr:UnhandledState|Req:NONE");
        break;
    }
}

// void onLongReleaseBTN0()
// {
//     Logger.info.println("BTN0 Long released");
// }

/******************
 * BTN1 Callbacks *
 ******************/
void onCountBTN1()
{
    gauge.indicateR((uint8_t)0b111);
}

void onPressBTN1()
{
    Logger.info.println("BTN1 pressed");
    switch (myDE1.getState())
    {
    case Sleep:
        Logger.info.println("Cur:Sleep|Req:Idle");
        sendStateRequest(Idle);
        break;
    case Idle:
    case SchedIdle:
        if (myDE1.getSubstate() == NoState)
        {
            Logger.info.println("Curr:Idle|Req:Steam");
            sendStateRequest(Steam);
        }
        break;
    case Espresso:
        Logger.info.println("Curr:Espresso|Req:SkipToNext");
        sendStateRequest(SkipToNext);
        break;
    case Steam:
        Logger.info.println("Curr:Steam|Req:SkipToNext");
        sendStateRequest(SkipToNext);
        break;
    case HotWater:
        Logger.info.println("Curr:HotWater|Req:Idle");
        sendStateRequest(Idle);
        break;
    case HotWaterRinse:
        Logger.info.println("Curr:HotWaterRinse|Req:Idle");
        sendStateRequest(Idle);
        break;
    case SteamRinse:
        Logger.info.println("Curr:SteamRinse|Req:Idle");
        sendStateRequest(Idle);
        break;
    case AirPurge:
        Logger.info.println("Curr:AirPurge|Req:Idle");
        sendStateRequest(Idle);
        break;
    default:
        Logger.info.println("Curr:UnhandledState|Req:NONE");
        break;
    }
}

void onDoublePressBTN1()
{
    Logger.info.println("BTN1 double pressed");
    switch (myDE1.getState())
    {
    case Sleep:
        Logger.info.println("Cur:Sleep|Req:Idle");
        sendStateRequest(Idle);
        break;
    case Idle:
    case SchedIdle:
        if (myDE1.getSubstate() == NoState)
        {
            Logger.info.println("Curr:Idle|Req:SteamRinse");
            sendStateRequest(SteamRinse);
        }
        break;
    default:
        Logger.info.println("Curr:UnhandledState|Req:NONE");
        break;
    }
}

void onLongPressBTN1()
{
    Logger.info.println("BTN1 Long pressed");
    gauge.indicateR((uint8_t)0b111);
    switch (myDE1.getState())
    {
    case Sleep:
        Logger.info.println("Cur:Sleep|Req:Idle");
        sendStateRequest(Idle);
        break;
    case Idle:
    case SchedIdle:
        if (myDE1.getSubstate() == NoState)
        {
            Logger.info.println("Curr:Idle|Req:HotWater");
            sendStateRequest(HotWater);
        }
        break;
    case Espresso:
        Logger.info.println("Curr:Espresso|Req:Idle");
        sendStateRequest(Idle);
        break;
    case Steam:
        Logger.info.println("Curr:Steam|Req:Idle");
        sendStateRequest(Idle);
        break;
    case HotWater:
        Logger.info.println("Curr:HotWater|Req:Idle");
        sendStateRequest(Idle);
        break;
    case HotWaterRinse:
        Logger.info.println("Curr:HotWaterRinse|Req:Idle");
        sendStateRequest(Idle);
        break;
    case SteamRinse:
        Logger.info.println("Curr:SteamRinse|Req:Idle");
        sendStateRequest(Idle);
        break;
    case AirPurge:
        Logger.info.println("Curr:AirPurge|Req:Idle");
        sendStateRequest(Idle);
        break;
    default:
        Logger.info.println("Curr:UnhandledState|Req:NONE");
        break;
    }
}

// void onLongReleaseBTN1()
// {
//     Logger.info.println("BTN1 Long released");
// }