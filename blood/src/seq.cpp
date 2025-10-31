/*
 * Copyright (C) 2018, 2022 nukeykt
 *
 * This file is part of Blood-RE.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <stdlib.h>
#include <string.h>
#include "typedefs.h"
#include "build.h"
#include "callback.h"
#include "db.h"
#include "debug4g.h"
#include "error.h"
#include "eventq.h"
#include "globals.h"
#include "levels.h"
#include "loadsave.h"
#include "resource.h"
#include "seq.h"
#include "sfx.h"
#include "tile.h"

#if APPVER_BLOODREV >= AV_BR_BL120
#define LDIFF1 0
#define LDIFF3 0
#define LDIFF4 0
#define LDIFF5 0
#define LDIFF6 0
#define LDIFF2 0
#elif APPVER_BLOODREV >= AV_BR_BL111
#define LDIFF1 -1
#define LDIFF3 -1
#define LDIFF4 -1
#define LDIFF5 -1
#define LDIFF6 -1
#define LDIFF2 -20
#else
#define LDIFF1 -1
#define LDIFF3 -25
#define LDIFF4 -32
#define LDIFF5 -36
#define LDIFF6 -15
#define LDIFF2 -34
#endif

SEQINST siWall[kMaxXWalls];
SEQINST siMasked[kMaxXWalls];
SEQINST siCeiling[kMaxXSectors];
SEQINST siFloor[kMaxXSectors];
SEQINST siSprite[kMaxXSprites];

#define kMaxClients 256
#define kMaxSequences 1024

static ACTIVE activeList[kMaxSequences];
static int activeCount = 0;
static void(*clientCallback[kMaxClients])(int, int);
static int nClients = 0;

int seqRegisterClient(void(*pClient)(int, int))
{
    dassert(nClients < kMaxClients, 44+LDIFF1);
    int id = nClients++;
    clientCallback[id] = pClient;
    return id;
}

void Seq::Preload(void)
{
    if (memcmp(signature, "SEQ\x1a", 4) != 0)
        ThrowError(53+LDIFF1)("Invalid sequence");
    if ((version&0xff00) != 0x300)
        ThrowError(56+LDIFF1)("Obsolete sequence version");
    for (int i = 0; i < nFrames; i++)
        tilePreloadTile(frames[i].tile);
}

void Seq::Precache(void)
{
    if (memcmp(signature, "SEQ\x1a", 4) != 0)
        ThrowError(66+LDIFF1)("Invalid sequence");
    if ((version&0xff00) != 0x300)
        ThrowError(69+LDIFF1)("Obsolete sequence version");
    for (int i = 0; i < nFrames; i++)
        tilePrecacheTile(frames[i].tile);
}

void seqCache(int id)
{
    DICTNODE *hSeq = gSysRes.Lookup(id, "SEQ");
    if (!hSeq)
        return;
    Seq *pSeq = (Seq*)gSysRes.Lock(hSeq);
    pSeq->Precache();
    gSysRes.Unlock(hSeq);
}

static void UpdateSprite(int nXSprite, SEQFRAME *pFrame)
{
    dassert(nXSprite > 0 && nXSprite < kMaxXSprites, 114+LDIFF1);
    int nSprite = xsprite[nXSprite].reference;
    dassert(nSprite >= 0 && nSprite < kMaxSprites, 116+LDIFF1);
    SPRITE *pSprite = &sprite[nSprite];
    dassert(pSprite->extra == nXSprite, 118+LDIFF1);
    if (pSprite->flags & kSpriteFlag1)
    {
        if (tilesizy[pSprite->picnum] != tilesizy[pFrame->tile] || picanm[pSprite->picnum].yoffset != picanm[pFrame->tile].yoffset
            || (pFrame->at3_0 && pFrame->at3_0 != pSprite->yrepeat))
            pSprite->flags |= kSpriteFlag2;
    }
    pSprite->picnum = pFrame->tile;
    if (pFrame->at5_0)
        pSprite->pal = pFrame->at5_0;
    pSprite->shade = pFrame->at4_0;
    if (pFrame->at2_0)
        pSprite->xrepeat = pFrame->at2_0;
    if (pFrame->at3_0)
        pSprite->yrepeat = pFrame->at3_0;
    if (pFrame->at1_4)
        pSprite->cstat |= 2;
    else
        pSprite->cstat &= ~2;
    if (pFrame->at1_5)
        pSprite->cstat |= 512;
    else
        pSprite->cstat &= ~512;
    if (pFrame->at1_6)
        pSprite->cstat |= 1;
    else
        pSprite->cstat &= ~1;
    if (pFrame->at1_7)
        pSprite->cstat |= 256;
    else
        pSprite->cstat &= ~256;
    if (pFrame->at6_2)
        pSprite->cstat |= 32768;
    else
        pSprite->cstat &= ~32768;
    if (pFrame->at6_0)
        pSprite->cstat |= 4096;
    else
        pSprite->cstat &= ~4096;
    if (pFrame->at5_6)
        pSprite->flags |= kSpriteFlag8;
    else
        pSprite->flags &= ~kSpriteFlag8;
    if (pFrame->at5_7)
        pSprite->flags |= kSpriteFlag3;
    else
        pSprite->flags &= ~kSpriteFlag3;
    if (pFrame->at6_3)
        pSprite->flags |= kSpriteFlag10;
    else
        pSprite->flags &= ~kSpriteFlag10;
    if (pFrame->at6_4)
        pSprite->flags |= kSpriteFlag11;
    else
        pSprite->flags &= ~kSpriteFlag11;
}

static void UpdateWall(int nXWall, SEQFRAME *pFrame)
{
    dassert(nXWall > 0 && nXWall < kMaxXWalls, 194+LDIFF1);
    int nWall = xwall[nXWall].reference;
    dassert(nWall >= 0 && nWall < kMaxWalls, 196+LDIFF1);
    WALL *pWall = &wall[nWall];
    dassert(pWall->extra == nXWall, 198+LDIFF1);
    pWall->picnum = pFrame->tile;
    if (pFrame->at5_0)
        pWall->pal = pFrame->at5_0;
    if (pFrame->at1_4)
        pWall->cstat |= 128;
    else
        pWall->cstat &= ~128;
    if (pFrame->at1_5)
        pWall->cstat |= 512;
    else
        pWall->cstat &= ~512;
    if (pFrame->at1_6)
        pWall->cstat |= 1;
    else
        pWall->cstat &= ~1;
    if (pFrame->at1_7)
        pWall->cstat |= 64;
    else
        pWall->cstat &= ~64;
}

static void UpdateMasked(int nXWall, SEQFRAME *pFrame)
{
    dassert(nXWall > 0 && nXWall < kMaxXWalls, 229+LDIFF1);
    int nWall = xwall[nXWall].reference;
    dassert(nWall >= 0 && nWall < kMaxWalls, 231+LDIFF1);
    WALL *pWall = &wall[nWall];
    dassert(pWall->extra == nXWall, 233+LDIFF1);
    dassert(pWall->nextwall >= 0, 234+LDIFF1);
    WALL *pWallNext = &wall[pWall->nextwall];
    pWall->overpicnum = pWallNext->overpicnum = pFrame->tile;
    if (pFrame->at5_0)
        pWall->pal = pWallNext->pal = pFrame->at5_0;
    if (pFrame->at1_4)
    {
        pWall->cstat |= 128;
        pWallNext->cstat |= 128;
    }
    else
    {
        pWall->cstat &= ~128;
        pWallNext->cstat &= ~128;
    }
    if (pFrame->at1_5)
    {
        pWall->cstat |= 512;
        pWallNext->cstat |= 512;
    }
    else
    {
        pWall->cstat &= ~512;
        pWallNext->cstat &= ~512;
    }
    if (pFrame->at1_6)
    {
        pWall->cstat |= 1;
        pWallNext->cstat |= 1;
    }
    else
    {
        pWall->cstat &= ~1;
        pWallNext->cstat &= ~1;
    }
    if (pFrame->at1_7)
    {
        pWall->cstat |= 64;
        pWallNext->cstat |= 64;
    }
    else
    {
        pWall->cstat &= ~64;
        pWallNext->cstat &= ~64;
    }
}

static void UpdateFloor(int nXSector, SEQFRAME *pFrame)
{
    dassert(nXSector > 0 && nXSector < kMaxXSectors, 290+LDIFF1);
    int nSector = xsector[nXSector].reference;
    dassert(nSector >= 0 && nSector < kMaxSectors, 292+LDIFF1);
    SECTOR *pSector = &sector[nSector];
    dassert(pSector->extra == nXSector, 294+LDIFF1);
    pSector->floorpicnum = pFrame->tile;
    pSector->floorshade = pFrame->at4_0;
    if (pFrame->at5_0)
        pSector->floorpal = pFrame->at5_0;
}

static void UpdateCeiling(int nXSector, SEQFRAME *pFrame)
{
    dassert(nXSector > 0 && nXSector < kMaxXSectors, 305+LDIFF1);
    int nSector = xsector[nXSector].reference;
    dassert(nSector >= 0 && nSector < kMaxSectors, 307+LDIFF1);
    SECTOR *pSector = &sector[nSector];
    dassert(pSector->extra == nXSector, 309+LDIFF1);
    pSector->ceilingpicnum = pFrame->tile;
    pSector->ceilingshade = pFrame->at4_0;
    if (pFrame->at5_0)
        pSector->ceilingpal = pFrame->at5_0;
}

void SEQINST::Update(ACTIVE *pActive)
{
    dassert(frameIndex < pSequence->nFrames, 320+LDIFF1);
    switch (pActive->type)
    {
    case 0:
        UpdateWall(pActive->xindex, &pSequence->frames[frameIndex]);
        break;
    case 1:
        UpdateCeiling(pActive->xindex, &pSequence->frames[frameIndex]);
        break;
    case 2:
        UpdateFloor(pActive->xindex, &pSequence->frames[frameIndex]);
        break;
    case 3:
        UpdateSprite(pActive->xindex, &pSequence->frames[frameIndex]);
        if (pSequence->frames[frameIndex].at6_1)
        {
            SPRITE* pSprite = &sprite[xsprite[pActive->xindex].reference];
            sfxPlay3DSound(pSprite, pSequence->ata);
        }
        break;
    case 4:
        UpdateMasked(pActive->xindex, &pSequence->frames[frameIndex]);
        break;
    }
    if (pSequence->frames[frameIndex].at5_5 && atc != -1)
        clientCallback[atc](pActive->type, pActive->xindex);
}

SEQINST * GetInstance(int a1, int nXIndex)
{
#if APPVER_BLOODREV >= AV_BR_BL111
    switch (a1)
    {
    case 0:
        if (nXIndex > 0 && nXIndex < kMaxXWalls) return &siWall[nXIndex];
        break;
    case 1:
        if (nXIndex > 0 && nXIndex < kMaxXSectors) return &siCeiling[nXIndex];
        break;
    case 2:
        if (nXIndex > 0 && nXIndex < kMaxXSectors) return &siFloor[nXIndex];
        break;
    case 3:
        if (nXIndex > 0 && nXIndex < kMaxXSprites) return &siSprite[nXIndex];
        break;
    case 4:
        if (nXIndex > 0 && nXIndex < kMaxXWalls) return &siMasked[nXIndex];
        break;
    }
    return NULL;
#else
    switch (a1)
    {
    case 0:
        dassert(nXIndex > 0 && nXIndex < kMaxXWalls, 358);
        return &siWall[nXIndex];
    case 1:
        dassert(nXIndex > 0 && nXIndex < kMaxXSectors, 362);
        return &siCeiling[nXIndex];
    case 2:
        dassert(nXIndex > 0 && nXIndex < kMaxXSectors, 366);
        return &siFloor[nXIndex];
    case 3:
        dassert(nXIndex > 0 && nXIndex < kMaxXSprites, 370);
        return &siSprite[nXIndex];
    case 4:
        dassert(nXIndex > 0 && nXIndex < kMaxXWalls, 374);
        return &siMasked[nXIndex];
    }
    ThrowError(379)("GetInstance: unexpected object type");
    return NULL;
#endif
}

void UnlockInstance(SEQINST *pInst)
{
    dassert(pInst != NULL, 411+LDIFF3);
    dassert(pInst->hSeq != NULL, 412+LDIFF3);
    dassert(pInst->pSequence != NULL, 413+LDIFF3);
    gSysRes.Unlock(pInst->hSeq);
    pInst->hSeq = NULL;
    pInst->pSequence = NULL;
    pInst->at13 = 0;
}

void seqSpawn(int a1, int a2, int a3, int a4)
{
    SEQINST *pInst = GetInstance(a2,a3);
#if APPVER_BLOODREV >= AV_BR_BL111
    if (!pInst)
        return;
#endif
    DICTNODE *hSeq = gSysRes.Lookup(a1, "SEQ");
    if (!hSeq)
        ThrowError(435+LDIFF4)("Missing sequence #%d", a1);
    int i = activeCount;
    if (pInst->at13)
    {
        if (pInst->hSeq == hSeq)
            return;
        UnlockInstance(pInst);
        for (i = 0; i < activeCount; i++)
        {
            if (activeList[i].type == a2 && activeList[i].xindex == a3)
                break;
        }
        dassert(i < activeCount, 452+LDIFF4);
    }
    Seq *pSeq = (Seq*)gSysRes.Lock(hSeq);
    if (memcmp(pSeq->signature, "SEQ\x1a", 4) != 0)
        ThrowError(458+LDIFF4)("Invalid sequence %d", a1);
    if ((pSeq->version & 0xff00) != 0x300)
        ThrowError(461+LDIFF4)("Sequence %d is obsolete version", a1);
    pInst->hSeq = hSeq;
    pInst->pSequence = pSeq;
    pInst->at8 = a1;
    pInst->atc = a4;
    pInst->at13 = 1;
    pInst->at10 = pSeq->at8;
    pInst->frameIndex = 0;
    if (i == activeCount)
    {
        dassert(activeCount < kMaxSequences, 473+LDIFF4);
        activeList[activeCount].type = a2;
        activeList[activeCount].xindex = a3;
        activeCount++;
    }
    pInst->Update(&activeList[i]);
}

void seqKill(int a1, int a2)
{
    SEQINST *pInst = GetInstance(a1, a2);
#if APPVER_BLOODREV >= AV_BR_BL111
    if (!pInst)
        return;
#endif
    if (!pInst->at13)
        return;
    int i;
    for (i = 0; i < activeCount; i++)
    {
        if (activeList[i].type == a1 && activeList[i].xindex == a2)
            break;
    }
    dassert(i < activeCount, 499+LDIFF5);
    activeCount--;
    activeList[i] = activeList[activeCount];
    pInst->at13 = 0;
    UnlockInstance(pInst);
}

void seqKillAll(void)
{
    for (int i = 0; i < kMaxXWalls; i++)
    {
        if (siWall[i].at13)
            UnlockInstance(&siWall[i]);
        if (siMasked[i].at13)
            UnlockInstance(&siMasked[i]);
    }
    for (i = 0; i < kMaxXSectors; i++)
    {
        if (siCeiling[i].at13)
            UnlockInstance(&siCeiling[i]);
        if (siFloor[i].at13)
            UnlockInstance(&siFloor[i]);
    }
    for (i = 0; i < kMaxXSprites; i++)
    {
        if (siSprite[i].at13)
            UnlockInstance(&siSprite[i]);
    }
    activeCount = 0;
}

int seqGetStatus(int a1, int a2)
{
    SEQINST *pInst = GetInstance(a1, a2);
#if APPVER_BLOODREV >= AV_BR_BL111
    if (!pInst)
        return -1;
#endif
    if (pInst->at13)
        return pInst->frameIndex;
    return -1;
}

int seqGetID(int a1, int a2)
{
    SEQINST *pInst = GetInstance(a1, a2);
    if (pInst)
        return pInst->at8;
    return -1;
}

#if APPVER_BLOODREV < AV_BR_BL111
void seqMoveInstance(int a1, int a2, int a3)
{
    int i;
    for (i = 0; i < activeCount; i++)
    {
        if (activeList[i].type == a1 && activeList[i].xindex == a2)
        {
            activeList[i].xindex = a3;
            *GetInstance(a1, a3) = *GetInstance(a1, a2);
            break;
        }
    }
}
#endif

void seqProcess(int a1)
{
    for (int i = 0; i < activeCount; i++)
    {
        SEQINST *pInst = GetInstance(activeList[i].type, activeList[i].xindex);
        Seq *pSeq = pInst->pSequence;
        dassert(pInst->frameIndex < pSeq->nFrames, 594+LDIFF6);
        pInst->at10 -= a1;
        while (pInst->at10 < 0)
        {
            pInst->at10 += pSeq->at8;
            pInst->frameIndex++;
            if (pInst->frameIndex == pSeq->nFrames)
            {
                if (pSeq->atc & kSeqFlag0)
                    pInst->frameIndex = 0;
                else
                {
                    UnlockInstance(pInst);
                    if (pSeq->atc & kSeqFlag1)
                    {
                        switch (activeList[i].type)
                        {
                        case 3:
                        {
                            int nSprite = xsprite[activeList[i].xindex].reference;
                            dassert(nSprite >= 0 && nSprite < kMaxSprites, 618+LDIFF6);
                            evKill(nSprite, 3);
#if APPVER_BLOODREV >= AV_BR_BL120
                            if ((sprite[nSprite].flags & kSpriteFlag4) && sprite[nSprite].inittype >= 200 && sprite[nSprite].inittype < 254)
                                evPost(nSprite, 3, gGameOptions.nMonsterRespawnTime, CALLBACK_ID_9);
                            else
#endif
                                DeleteSprite(nSprite);
                            break;
                        }
                        case 4:
                        {
                            int nWall = xwall[activeList[i].xindex].reference;
                            dassert(nWall >= 0 && nWall < kMaxWalls, 649+LDIFF2);
                            wall[nWall].cstat &= ~(8+16+32);
                            //int nNextWall = wall[nWall].nextwall;
                            if (wall[nWall].nextwall != -1)
                                wall[wall[nWall].nextwall].cstat &= ~(8+16+32);
                            break;
                        }
                        }
                    }
                    activeList[i--] = activeList[--activeCount];
                    break;
                }
            }
            pInst->Update(&activeList[i]);
        }
    }
}

class SeqLoadSave : public LoadSave {
    virtual void Load(void);
    virtual void Save(void);
};

void SeqLoadSave::Load(void)
{
    int i, j;
    Read(&siWall, sizeof(siWall));
    Read(&siMasked, sizeof(siMasked));
    Read(&siCeiling, sizeof(siCeiling));
    Read(&siFloor, sizeof(siFloor));
    Read(&siSprite, sizeof(siSprite));
    Read(&activeList, sizeof(activeList));
    Read(&activeCount, sizeof(activeCount));
    for (i = 0; i < kMaxXWalls; i++)
    {
        siWall[i].hSeq = NULL;
        siWall[i].pSequence = NULL;
        siMasked[i].hSeq = NULL;
        siMasked[i].pSequence = NULL;
    }
    for (i = 0; i < kMaxXSectors; i++)
    {
        siCeiling[i].hSeq = NULL;
        siCeiling[i].pSequence = NULL;
        siFloor[i].hSeq = NULL;
        siFloor[i].pSequence = NULL;
    }
    for (i = 0; i < kMaxXSprites; i++)
    {
        siSprite[i].hSeq = NULL;
        siSprite[i].pSequence = NULL;
    }
    for (j = 0; j < activeCount; j++)
    {
        int type = activeList[j].type;
        int xindex = activeList[j].xindex;
        SEQINST *pInst = GetInstance(type, xindex);
        if (pInst->at13)
        {
            int nSeq = pInst->at8;
            DICTNODE *hSeq = gSysRes.Lookup(nSeq, "SEQ");
            if (!hSeq)
                ThrowError(735+LDIFF2)("Missing sequence #%d", nSeq);
            Seq *pSeq = (Seq*)gSysRes.Lock(hSeq);
            if (memcmp(pSeq->signature, "SEQ\x1a", 4) != 0)
                ThrowError(740+LDIFF2)("Invalid sequence %d", nSeq);
            if ((pSeq->version & 0xff00) != 0x300)
                ThrowError(743+LDIFF2)("Sequence %d is obsolete version", nSeq);
            pInst->hSeq = hSeq;
            pInst->pSequence = pSeq;
        }
    }
}

void SeqLoadSave::Save(void)
{
    Write(&siWall, sizeof(siWall));
    Write(&siMasked, sizeof(siMasked));
    Write(&siCeiling, sizeof(siCeiling));
    Write(&siFloor, sizeof(siFloor));
    Write(&siSprite, sizeof(siSprite));
    Write(&activeList, sizeof(activeList));
    Write(&activeCount, sizeof(activeCount));
}

static SeqLoadSave myLoadSave;
