#pragma once
#include "sigscanner.h"
#include "game.h"


struct Offset
{
    std::string moduleName;
    int offset;
    int address;
    std::string signature;
    int sigOffset;

    Offset(std::string moduleName, int currentOffset, std::string signature, int sigOffset = 0)
    {
        this->moduleName = moduleName;
        this->offset = currentOffset;
        this->signature = signature;
        this->sigOffset = sigOffset;

        int newOffset = SigScanner::VerifyOffset(moduleName, currentOffset, signature, sigOffset);
        if (newOffset > 0)
        {
            this->offset = newOffset;
        }
            
        if (newOffset == -1)
        {
            Game::errorMsg(("Signature not found: " + signature).c_str());
            return;
        }

        this->address = (uintptr_t)GetModuleHandle(moduleName.c_str()) + this->offset;
    }
};

class Offsets
{
public:
    Offset GetFullScreenTexture =        { "client.dll", 0x1A83F0, "A1 ? ? ? ? 85 C0 75 53 8B 0D ? ? ? ? 8B 01 8B 90 ? ? ? ? 6A 00 6A 01 68 ? ? ? ? 68 ? ? ? ? FF D2 50 B9 ? ? ? ? E8 ? ? ? ? 80 3D ? ? ? ? ? 75 1C 8B 0D ? ? ? ? 8B 01 8B 90 ? ? ? ? 68 ? ? ? ? C6 05 ? ? ? ? ? FF D2 A1 ? ? ? ? C3" };
    Offset RenderView =                  { "client.dll", 0x1F2120, "55 8B EC 83 EC 2C 53 56 8B F1 6A 00 8D 8E ? ? ? ? E8 ? ? ? ?" };
    Offset g_pClientMode =               { "client.dll", 0x2580D0, "8B 0D ? ? ? ? 8B 01 8B 90 ? ? ? ? FF D2 8B 04 85 ? ? ? ? C3", 2 };
    Offset CalcViewModelView =           { "client.dll", 0x27D750, "55 8B EC 83 EC 34 53 8B D9 80 BB" };
    Offset CreateMove = { "client.dll", 0xD2B60, "55 8B EC 51 56 6A 00 6A 01 8D 4D FF E8 ? ? ? ? " };
    //55 8B EC 51 56 6A 00 6A 01 8D 4D FF E8 ? ? ? ? 

    //Offset WriteUsercmdDeltaToBuffer =   { "client.dll", 0x134790, "55 8B EC 83 EC 60 0F 57 C0 8B 55 0C" }; //
    Offset WriteUsercmd =                { "client.dll", 0x1C2060, "55 8B EC A1 ? ? ? ? 83 78 30 00 53 8B 5D 0C 56 57" };
    Offset g_pppInput =                  { "client.dll", 0xD12A0, "8B 0D ? ? ? ? 8B 01 8B 50 68 FF E2", 2 };
    Offset AdjustEngineViewport =        { "client.dll", 0x41AD10, "55 8B EC 8B 0D ? ? ? ? 85 C9 74 17" };
    Offset IsSplitScreen =               { "client.dll", 0x1B2A60, "33 C0 83 3D ? ? ? ? ? 0F 9D C0" };
    Offset PrePushRenderTarget =         { "client.dll", 0xA8C80, "55 8B EC 8B C1 56 8B 75 08 8B 0E 89 08 8B 56 04 89" };

    Offset ReadUserCmd =                 { "server.dll", 0x205100, "55 8B EC 53 8B 5D 10 56 57 8B 7D 0C 53" };
    Offset ProcessUsercmds =             { "server.dll", 0x170300, "55 8B EC B8 ? ? ? ? E8 ? ? ? ? 0F 57 C0 53 56 57 B9 ? ? ? ? 8D 85 ? ? ? ? 33 DB" }; //?
    Offset CBaseEntity_entindex =        { "server.dll", 0x39F00, "8B 41 1C 85 C0 75 01 C3 8B 0D ? ? ? ? 2B 41 58 C1 F8 04 C3 CC"};
    Offset EyePosition =                 { "server.dll", 0xF40E0, "55 8B EC 56 8B F1 8B 86 ? ? ? ? C1 E8 0B A8 01 74 05 E8 ? ? ? ? 8B 45 08 F3" };

    Offset GetRenderTarget =             { "materialsystem.dll", 0x2CD30, "83 79 4C 00" };
    Offset Viewport =                    { "materialsystem.dll", 0x2E010, "55 8B EC 8B 45 0C 53 8B 5D" };
    Offset GetViewport =                 { "materialsystem.dll", 0x2CAF0, "55 8B EC 8B 41 4C 8B 49 40 8D 04 C0 83 7C 81 ? ?" };
    Offset PushRenderTargetAndViewport = { "materialsystem.dll", 0x2D5F0, "55 8B EC 83 EC 24 8B 45 08 8B 55 10 89" };
    Offset PopRenderTargetAndViewport =  { "materialsystem.dll", 0x2CE80, "56 8B F1 83 7E 4C 00" };

    Offset Weapon_ShootPosition =        { "client.dll", 0x2A8A60, "55 8B EC 8B 01 8B 90 ? ? ? ? 56 8B 75 08 56 FF D2 8B C6 5E 5D C2 04 00" };

    Offset TraceFirePortalClient =       { "client.dll", 0x3E0980, "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 56 57 8B F1 6A" };
    Offset TraceFirePortalServer =       { "server.dll", 0x400D50, "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 56 57 8B F1 6A" };

    //53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 56 57 8B F1 6A 00 56 8D 4D C0 89 75 F8 E8
    //Offset DrawModelExecute =            { "engine.dll", 0xE05E0, "55 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 33 C5 89 45 FC 8B 45 10 56 8B 75 08 57 8B" }; //
    Offset VGui_Paint =                  { "engine.dll", 0x115CE0, "55 8B EC E8 ? ? ? ? 8B 10 8B C8 8B 52 38" };

    Offset DrawSelf = { "client.dll", 0x12CC90, "55 8B EC 56 8B F1 80 BE ? ? ? ? ? 0F 84 ? ? ? ? 8B 0D" };
    Offset ClipTransform = { "client.dll", 0x1DD130, "55 8B EC 8B 0D ? ? ? ? 8B 01 8B 90 ? ? ? ? FF D2 8B 4D" };

    Offset PlayerPortalled = { "client.dll", 0x27C9D0, "55 8B EC 83 EC 78 53 56 8B D9 8B 0D ? ? ? ? 8B 01 8B 90 ? ? ? ? 57 33 FF 57 FF D2" };
};