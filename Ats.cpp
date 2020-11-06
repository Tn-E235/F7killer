#include "stdafx.h"
#include "atsplugin.h"
#include "Ats.h"
#include <cmath>

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) { return TRUE; }
ATS_API int WINAPI GetPluginVersion() { return ATS_VERSION; }
ATS_API void WINAPI SetVehicleSpec(ATS_VEHICLESPEC vehicleSpec) {
	g_svcBrake = vehicleSpec.BrakeNotches;
	g_emgBrake = g_svcBrake + 1;
}
ATS_API void WINAPI Initialize(int brake) { 
	g_speed = 0;
	g_position = 0.0;
}
ATS_API ATS_HANDLES WINAPI Elapse(ATS_VEHICLESTATE vehicleState, int *panel, int *sound) {
	g_deltaT = vehicleState.Time - g_time;
	d_speed = vehicleState.Speed - g_speed;
	d_position = vehicleState.Location - g_position;

	g_time = vehicleState.Time;
	g_speed = vehicleState.Speed;
	g_position = vehicleState.Location;

	if (g_pilotlamp) { g_output.Reverser = g_reverser; }
	else { g_output.Reverser = 0; }
	g_output.Brake = g_brakeNotch;
	g_output.Power = g_powerNotch;
	g_output.ConstantSpeed = ATS_CONSTANTSPEED_CONTINUE;

	if (kill()) {
		MessageBox(NULL, TEXT("F7g‚Á‚Ä‚ñ‚¶‚á‚Ë[‚¼"),
			TEXT("Œx"),MB_OK | MB_ICONINFORMATION);
	}
    return g_output;
}

ATS_API void WINAPI SetPower(int notch) { g_powerNotch = notch; }
ATS_API void WINAPI SetBrake(int notch) { g_brakeNotch = notch; }
ATS_API void WINAPI SetReverser(int pos) { g_reverser = pos; }
ATS_API void WINAPI KeyDown(int atsKeyCode) {}
ATS_API void WINAPI KeyUp(int hornType) {}
ATS_API void WINAPI HornBlow(int atsHornBlowIndex) {}
ATS_API void WINAPI DoorOpen() { g_pilotlamp = false; }
ATS_API void WINAPI DoorClose() { g_pilotlamp = true; }
ATS_API void WINAPI SetSignal(int signal) {}
ATS_API void WINAPI SetBeaconData(ATS_BEACONDATA beaconData) {}
ATS_API void WINAPI Load() {}
ATS_API void WINAPI Dispose() {}
bool kill() {
	if (!g_pilotlamp) return false;
	if (d_position == g_position) return false;
	if (g_deltaT > 1000) return false;
	if (abs(d_speed) > 2.5) return true;
	return false;
}