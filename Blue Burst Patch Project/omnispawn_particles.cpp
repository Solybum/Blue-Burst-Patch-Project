#include "helpers.h"
#include "omnispawn.h"
#include <mathutil.h>
#include "map.h"

// TODO: Cleanup this file.

namespace Omnispawn
{
    using Map::MapType;

#define MAP_PARTICLEENTRY_SIZE  (0x2600)
// Helper macros to swap out the client's loaded map particle data.
#define GET_MAP_PARTICLEENTRY(a) ((a) = *(byte **)0xa72ccc)
#define SET_MAP_PARTICLEENTRY(a) (*(byte **)0xa72ccc = (a))
#define RESTORE_MAP_PARTICLEENTRY(a)  (SET_MAP_PARTICLEENTRY((a)))
#define REPLACE_MAP_PARTICLEENTRY(a, b)                   \
   do {                                                   \
       GET_MAP_PARTICLEENTRY((a));                        \
       if ((b))                                           \
       {                                                  \
           SET_MAP_PARTICLEENTRY((b));                    \
       }                                                  \
   } while (0)                                             


    // Index is map, element is pointer to particle data.
    // TODO: Structure for particles to make this nicer.
    static byte *omniMapParticleData[(size_t)MapType::MAX_INDEX + 1];

    // Load all particle entry files once.
    static void LoadAllMapParticleEntryFiles()
    {
        static int loaded = 0;
        if (loaded)
            return;

        char fn[128];

        // For each map, load particleentry file and store it into the global
        for (int iMap = 0; iMap < (size_t)MapType::MAX_INDEX; ++iMap)
        {
            // The game actually uses new here but it doesn't matter, we keep
            // this memory allocated for the lifetime of the client.
            byte *data = (byte *)malloc(MAP_PARTICLEENTRY_SIZE);

            sprintf_s(&fn[0], sizeof(fn), "particleentrya%-02d.dat", iMap);

            uint32_t(__cdecl * pf_load)(char *, byte *) = (uint32_t(__cdecl *)(char *, byte *))0x5b9910;
            (*pf_load)(&fn[0], data);

            omniMapParticleData[iMap] = data;
        }

        loaded = 1;
    }

    // Wraps the loading of particleentry.dat for particles 0-511 inclusive.
    // On first call, loads all of the map specific particles.
    static uint32_t __cdecl WrapLoadParticleEntry(void)
    {
        // First time here will load all map particles.
        LoadAllMapParticleEntryFiles();

        // Original function being wrapped.
        uint32_t(__cdecl *pfLoadOneMapParticleEntry)(void) = (uint32_t(__cdecl *)(void))0x50d9a0;
        uint32_t ret = (*pfLoadOneMapParticleEntry)();

        // Copy whatever the game loaded into our data so that particles are reloaded when entering maps.
        uint32_t currentMap = *(uint32_t *)0xaafc9c;

        byte *mapParticleEntryData = *(byte **)0xa72ccc;
        if (mapParticleEntryData)
        {
            memcpy_s(omniMapParticleData[currentMap], MAP_PARTICLEENTRY_SIZE,
                mapParticleEntryData, MAP_PARTICLEENTRY_SIZE);
        }

        return ret;
    }

    byte *__cdecl FUN_0050b510_WithMap(Vec3f *pos, int particleId, int mapId)
    {
        byte *save;

        REPLACE_MAP_PARTICLEENTRY(save, omniMapParticleData[mapId]);

        byte *(__cdecl * pf)(Vec3f *, int) = (byte * (__cdecl *)(Vec3f *, int))0x50b510;
        byte *ret = (*pf)(pos, particleId);

        RESTORE_MAP_PARTICLEENTRY(save);

        return ret;
    }

    byte *__cdecl FUN_00509dc4_WithMap(Vec3f *pos, Vec3f *pos2, int particleId, uint16_t flags, int mapId)
    {
        byte *save;

        REPLACE_MAP_PARTICLEENTRY(save, omniMapParticleData[mapId]);

        byte *(__cdecl * pf)(Vec3f *, Vec3f *, int, uint16_t) =
            (byte * (__cdecl *)(Vec3f *, Vec3f *, int, uint16_t))0x509dc4;
        byte *ret = (*pf)(pos, pos2, particleId, flags);

        RESTORE_MAP_PARTICLEENTRY(save);

        return ret;
    }

    byte *__cdecl FUN_00509590_WithMap(Vec3f *pos, int particleId, int mapId)
    {
        byte *save;

        REPLACE_MAP_PARTICLEENTRY(save, omniMapParticleData[mapId]);

        byte *(__cdecl * pf)(Vec3f *, int) =(byte * (__cdecl *)(Vec3f *, int))0x509590;
        byte *ret = (*pf)(pos, particleId);

        RESTORE_MAP_PARTICLEENTRY(save);

        return ret;
    }

    byte *__cdecl FUN_0050a664_WithMap(Vec3f *pos, Vec3f *pos2, int particleId, int param4, uint16_t flags, int mapId)
    {
        byte *save;

        REPLACE_MAP_PARTICLEENTRY(save, omniMapParticleData[mapId]);

        byte *(__cdecl * pf)(Vec3f *, Vec3f *, int, int, uint16_t) =
            (byte * (__cdecl *)(Vec3f *, Vec3f *, int, int, uint16_t))0x50a664;
        byte *ret = (*pf)(pos, pos2, particleId, param4, flags);

        RESTORE_MAP_PARTICLEENTRY(save);

        return ret;
    }

    byte *__cdecl FUN_0050bab4_WithMap(Vec3f *pos, Vec3f *pos2, int particleId, int mapId)
    {
        byte *save;

        REPLACE_MAP_PARTICLEENTRY(save, omniMapParticleData[mapId]);

        byte *(__cdecl * pf)(Vec3f *, Vec3f *, int) = (byte * (__cdecl *)(Vec3f *, Vec3f *, int))0x50bab4;
        byte *ret = (*pf)(pos, pos2, particleId);

        RESTORE_MAP_PARTICLEENTRY(save);

        return ret;
    }

    byte *__cdecl FUN_00509b24_WithMap(Vec3f *pos, Vec3f *pos2, int particleId, int mapId)
    {
        byte *save;

        REPLACE_MAP_PARTICLEENTRY(save, omniMapParticleData[mapId]);

        byte *(__cdecl * pf)(Vec3f *, Vec3f *, int) = (byte * (__cdecl *)(Vec3f *, Vec3f *, int))0x509b24;
        byte *ret = (*pf)(pos, pos2, particleId);

        RESTORE_MAP_PARTICLEENTRY(save);

        return ret;
    }

    byte *__cdecl FUN_0050b510_CCA(Vec3f *pos, int particleId) 
    {
        return FUN_0050b510_WithMap(pos, particleId, (int)MapType::CCA);
    }

    byte *__cdecl FUN_0050b510_Seabed(Vec3f *pos, int particleId) 
    {
        return FUN_0050b510_WithMap(pos, particleId, (int)MapType::Seabed_Upper);
    }

    byte *__cdecl FUN_00509dc4_CCA(Vec3f *pos, Vec3f *pos2, int particleId, uint16_t flags)
    {
        return FUN_00509dc4_WithMap(pos, pos2, particleId, flags, (int)MapType::CCA);
    }

    byte *__cdecl FUN_00509590_Seabed(Vec3f *pos, int particleId)
    {
        return FUN_00509590_WithMap(pos, particleId, (int)MapType::Seabed_Upper);
    }

    byte *__cdecl FUN_00509590_Tower(Vec3f *pos, int particleId)
    {
        return FUN_00509590_WithMap(pos, particleId, (int)MapType::Tower);
    }

    byte *__cdecl FUN_00509590_Wilds(Vec3f *pos, int particleId)
    {
        return FUN_00509590_WithMap(pos, particleId, (int)MapType::Wilds1);
    }

    byte *__cdecl FUN_00509590_Seabed0x20f_Tower0x223(Vec3f *pos, int particleId)
    {
        if (particleId == 0x20f)
            return FUN_00509590_WithMap(pos, particleId, (int)MapType::Seabed_Upper);
        else if (particleId == 0x223)
            return FUN_00509590_WithMap(pos, particleId, (int)MapType::Tower);

        // Particle 0x12f, doesn't matter which map.
        return FUN_00509590_WithMap(pos, particleId, (int)MapType::Tower);
    }

    byte *__cdecl FUN_0050a664_Seabed(Vec3f *pos, Vec3f *pos2, int particleId, int param4, uint16_t flags)
    {
        return FUN_0050a664_WithMap(pos, pos2, particleId, param4, flags, (int)MapType::Seabed_Upper);
    }

    byte *__cdecl FUN_0050a664_Wilds(Vec3f *pos, Vec3f *pos2, int particleId, int param4, uint16_t flags)
    {
        return FUN_0050a664_WithMap(pos, pos2, particleId, param4, flags, (int)MapType::Wilds1);
    }

    byte *__cdecl FUN_0050b510_Wilds(Vec3f *pos, int particleId) 
    {
        return FUN_0050b510_WithMap(pos, particleId, (int)MapType::Wilds1);
    }

    byte *__cdecl FUN_00509dc4_Seabed(Vec3f *pos, Vec3f *pos2, int particleId, uint16_t flags)
    {
        return FUN_00509dc4_WithMap(pos, pos2, particleId, flags, (int)MapType::Seabed_Upper);
    }

    byte *__cdecl FUN_0050bab4_Tower(Vec3f *pos1, Vec3f *pos2, int particleId)
    {
        return FUN_0050bab4_WithMap(pos1, pos2, particleId, (int)MapType::Tower);
    }

    byte *__cdecl FUN_00509b24_Tower(Vec3f *pos1, Vec3f *pos2, int particleId)
    {
        return FUN_00509b24_WithMap(pos1, pos2, particleId, (int)MapType::Tower);
    }

    void PatchCCAMonsterParticles()
    {
        // Mericarol/Merikle/Mericus spit
        PatchCALL(0x573fc8, 0x573fcd, (int)FUN_00509dc4_CCA);
        PatchCALL(0x573f7d, 0x573f82, (int)FUN_00509dc4_CCA);
        // Mericarol/Merikle/Mericus Megid Cloud and spores
        PatchCALL(0x5768c5, 0x5768ca, (int)FUN_0050b510_CCA);
        PatchCALL(0x5768f6, 0x5768fb, (int)FUN_0050b510_CCA);
        // Gi Gue bomb 'muzzle' effect on Gi Gue
        PatchCALL(0x56a85f, 0x56a864, (int)FUN_0050b510_CCA);
        // Gi Gue confuse ball attack
        PatchCALL(0x56ce67, 0x56ce6c, (int)FUN_00509dc4_CCA);
        PatchCALL(0x56ce84, 0x56ce84, (int)FUN_00509dc4_CCA);
        // Gi Gue death particles
        PatchCALL(0x56b1fb, 0x56b200, (int)FUN_0050b510_CCA);
        PatchCALL(0x56b20a, 0x56b20f, (int)FUN_0050b510_CCA);
    }

    void PatchSeabedMonsterParticles()
    {
        // Dolmdarl snare particles
        PatchCALL(0x552f2f, 0x552f34, (int)FUN_0050b510_Seabed);
        // Recon floating particle
        PatchCALL(0x58a5b7, 0x58a5bc, (int)FUN_00509590_Seabed);
        // Delbiter roar
        PatchCALL(0x54bc4e, 0x54bc53, (int)FUN_00509590_Seabed);
        // Delbiter shock/megid particles from roar
        PatchCALL(0x54b0f0, 0x54b0f5, (int)FUN_0050b510_Seabed);
        PatchCALL(0x54b0ff, 0x54b104, (int)FUN_0050b510_Seabed);
        PatchCALL(0x54b028, 0x54b02d, (int)FUN_0050b510_Seabed);
        PatchCALL(0x54b037, 0x54b03c, (int)FUN_0050b510_Seabed);
        // Delbiter laser related
        PatchCALL(0x54e073, 0x54e078, (int)FUN_0050a664_Seabed);
        PatchCALL(0x54b6c1, 0x54b6c6, (int)FUN_0050b510_Seabed);
        // Delbiter particle that follows after a roar
        PatchCALL(0x54aa26, 0x54aa2b, (int)FUN_00509590_Seabed);
        PatchCALL(0x54a9ec, 0x54a9f1, (int)FUN_00509590_Seabed);
        PatchCALL(0x54a9cd, 0x54a9d2, (int)FUN_00509590_Seabed);
        // Morfos laser
        PatchCALL(0x57ba35, 0x57ba3a, (int)FUN_0050b510_Seabed);
        PatchCALL(0x57d1dd, 0x57d1e2, (int)FUN_00509dc4_Seabed);
        // Morfos shield particles
        PatchCALL(0x57a041, 0x57a046, (int)FUN_0050b510_Seabed);
        PatchCALL(0x57a04c, 0x57a051, (int)FUN_0050b510_Seabed);
        // Morfos spinning laser particle
        PatchCALL(0x57aeb5, 0x57aeba, (int)FUN_0050b510_Seabed);
    }

    void PatchTowerMonsterParticles()
    {
        // Ill Gill - Charge particles
        PatchCALL(0x52fb3d, 0x52fb42, (int)FUN_00509590_Tower);
        PatchCALL(0x52fb1b, 0x52fb20, (int)FUN_00509590_Tower);
        // Ill Gill - Snare/root particles from the Ill Gill
        PatchCALL(0x52f035, 0x52f03a, (int)FUN_00509b24_Tower);
        PatchCALL(0x52ef94, 0x52ef99, (int)FUN_0050bab4_Tower);
        // Epsilon - epsigard shield particle
        PatchCALL(0x55b77d, 0x55b782, (int)FUN_00509590_Tower);
        // Epsilon - epsigard closing particles
        PatchCALL(0x55926f, 0x559274, (int)FUN_00509590_Tower);
        PatchCALL(0x558fa4, 0x558fa9, (int)FUN_00509590_Tower);
    }

    void PatchWildsMonsterParticles()
    {
        // Dorphon also shares some particles with Delbiter
        // Dorphon - I forget what these are.
        PatchCALL(0x5a6b8a, 0x5a6b8f, (int)FUN_00509590_Wilds);
        PatchCALL(0x5a6b68, 0x5a6b6d, (int)FUN_00509590_Wilds);
        PatchCALL(0x5a6b49, 0x5a6b4e, (int)FUN_00509590_Wilds);
        // Dorphon - Laser attack particles
        PatchCALL(0x54e073, 0x54e078, (int)FUN_0050a664_Wilds);
        PatchCALL(0x5a77b7, 0x5a77bc, (int)FUN_0050b510_Wilds);
        // Dorphon - "Vacuum" effect after being stunned
        PatchCALL(0x5a7ed9, 0x5a7ede, (int)FUN_00509590_Wilds);
    }


    void PatchMonsterParticlesSharedAreas()
    {
        // Dolmdarl and Ill Gill share this call for particle
        // rendered at their snared/rooted target.
        PatchCALL(0x579031, 0x579036, (int)FUN_00509590_Seabed0x20f_Tower0x223);
    }

    void PatchOmnispawnParticles()
    {
        // Wrap loading of particleentry.dat to load all map particles on first call.
        *(uint32_t *)0x9f8558 = (uint32_t)WrapLoadParticleEntry;

        PatchCCAMonsterParticles();
        PatchSeabedMonsterParticles();
        PatchTowerMonsterParticles();
        PatchWildsMonsterParticles();
        PatchMonsterParticlesSharedAreas();
    }

}