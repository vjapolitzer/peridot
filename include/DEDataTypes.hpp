#ifndef _DE_DATA_TYPES_HPP_
#define _DE_DATA_TYPES_HPP_

#include <Arduino.h>
enum DE_MachineState_t : uint8_t
{
  // These are reported states guaranteed by the API. The firmware will lie like a
  // cheap watch to maintain the illusion of these states.
  Sleep,      // 0 Everything is off
  GoingToSleep, // 1
  Idle,       // 2 Heaters are controlled, tank water will be heated if required.
  Busy,       // 3 Firmware is doing something you can't interrupt (eg. cooling down water heater after a shot, calibrating sensors on startup).
  Espresso,   // 4 Making espresso
  Steam,      // 5 Making steam
  HotWater,   // 6 Making hot water
  ShortCal,   // 7 Running a short calibration
  SelfTest,   // 8 Checking as much as possible within the firmware. Probably only used during manufacture or repair.
  LongCal,    // 9 Long and involved calibration, possibly involving user interaction. (See substates below, for cases like that).
  Descale,    // A Descale the whole bang-tooty
  FatalError, // B Something has gone horribly wrong
  Init,       // C Machine has not been run yet
  NoRequest,  // D State for T_RequestedState. Means nothing is specifically requested
  SkipToNext, // E In Espresso, skip to next frame. Others, go to Idle if possible
  HotWaterRinse, // F Produce hot water at whatever temperature is available
  SteamRinse,    // 10 Produce a blast of steam
  Refill,        // 11 Attempting, or needs, a refill.
  Clean,         // 12 Clean group head
  InBootLoader,  // 13 The main firmware has not run for some reason. Bootloader is active.
  AirPurge,      // 14 Air purge.
  SchedIdle      // 15 Scheduled wake up idle state
};

enum DE_MachineSubstate_t : uint8_t {
  NoState,          // 0 State is not relevant
  HeatWaterTank,    // 1 Cold water is not hot enough. Heating hot water tank.
  HeatWaterHeater,  // 2 Warm up hot water heater for shot.
  StabilizeMixTemp, // 3 Stabilize mix temp and get entire water path up to temperature.
  PreInfuse,        // 4 Espresso only. Hot Water and Steam will skip this state.
  Pour,             // 5 Not used in Steam
  Flush,            // 6 Espresso only, atm
  Steaming,         // 7 Steam only
  DescaleInit,      // Starting descale
  DescaleFillGroup, // get some descaling solution into the group and let it sit
  DescaleReturn,    // descaling internals
  DescaleGroup,     // descaling group
  DescaleSteam,     // descaling steam
  CleanInit,        // Starting clean
  CleanFillGroup,   // Fill the group
  CleanSoak,        // Wait for 60 seconds so we soak the group head
  CleanGroup,       // Flush through group
  PausedRefill,     // Have we given up on a refill
  PausedSteam,      // Are we paused in steam?

  // Error states
  Error_NaN=200,        // Something died with a NaN
  Error_Inf=201,        // Something died with an Inf
  Error_Generic=202,    // An error for which we have no more specific description
  Error_ACC=203,        // ACC not responding, unlocked, or incorrectly programmed
  Error_TSensor=204,    // We are getting an error that is probably a broken temperature sensor
  Error_PSensor=205,    // Pressure sensor error
  Error_WLevel=206,     // Water level sensor error
  Error_DIP=207,        // DIP switches told us to wait in the error state.
  Error_Assertion=208,  // Assertion failed
  Error_Unsafe=209,     // Unsafe value assigned to variable
  Error_InvalidParm=210,// Invalid parameter passed to function
  Error_Flash=211,      // Error accessing external flash
  Error_OOM=212,        // Could not allocate memory
  Error_Deadline=213    // Realtime deadline missed
};
/*
#define PACKEDATTR __attribute__((__packed__))
// This is a 16 bit struct that we store an 8.8 fractional number within.
// We wrap this simple 16 bit type in a struct to defeat C's self-defeating
// lack of type checking between typedefs of first class types. No struct =
// no type checking.
 
struct PACKEDATTR U16P8 {
  uint16_t value;
};

struct PACKEDATTR U16P12 {
  uint16_t value;
};

struct PACKEDATTR U8P4 {
  uint8_t value;
};

struct PACKEDATTR U8P0 {
  uint8_t value;
};

struct PACKEDATTR U24P16 {
  uint8_t Hi;
  uint8_t Mid;
  uint8_t Lo;
};

struct PACKEDATTR DE_ShotState_t {
  // Instant snapshot during a shot
  U16P12 GroupPressure; // Pressure at group
  U16P12 GroupFlow;     // Estimated Flow at group
  U16P8  MixTemp;       // Water Temperature entering group
  U24P16 HeadTemp;      // Temperature of water at showerhead
  U16P8  SetMixTemp;    // Set temperature. 0 if no target.
  U16P8  SetHeadTemp;   // Set shower head temp. 0 if no target.
  U8P4   SetGroupPressure; // Set pressure. 0 if not set.
  U8P4   SetGroupFlow;  // Set flow. 0 if not set.
  U8P0   FrameNumber;   // Frame we are currently in.
  U8P0   SteamTemp;     // Steam metal temp
};

struct PACKEDATTR DE_ShotSample_t {
  uint16_t SampleTime;   // Time since start of shot, in halfcycles
  DE_ShotState_t State;
};

struct PACKEDATTR DE_StateInfo_t {
  DE_MachineState_t State;
  DE_MachineSubstate_t SubState;
};
*/
#endif