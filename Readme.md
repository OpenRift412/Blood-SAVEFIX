# Blood-SAVEFIX
Nukeykt's Blood src reconstruction with the addition of tmyqlfpir's difficulty savegame fix, backported from NBlood. 
Main goal is to fix this infamous bug while maintaining maximum compatibility with online play.

If there are any issues with this fix's implementation, PLEASE open an issue to let me know! I'm not
much of a programmer, but I'll run it by more knowledgeable folks if I can!

**So what is this bug, exactly?**
In vanilla Blood (all versions on DOS have this issue), there is an issue when loading a saved game that causes
the damage scaling to use the incorrect values for the current difficulty, with the Lightly Broiled (medium)
difficulty being the only one not affected.

In vanilla, damage scaling is swapped as follows:
 - Still Kicking -------> Extra Crispy
 - Pink on the Inside --> Well Done
 - Lightly Broiled -----> Lightly Broiled (unaffected)
 - Well Done -----------> Pink on the Inside
 - Extra Crispy --------> Still Kicking

This fork of the source reconstruction fixes this, while also remaining demo and network compatible.

# Blood-RE
Matching decompilation of various versions Blood (1997) by nukeykt.

## Covered Blood versions
* Shareware 1.10
* Shareware 1.11 (19 August 1997)
* Shareware 1.11 (23 September 1997)
* Retail Shareware 1.10
* Retail Shareware 1.11 (19 August 1997)
* Retail Shareware 1.11 (23 September 1997)
* Registered 1.10
* Registered 1.11 (19 August 1997)
* Registered 1.11 (23 September 1997)
* Registered 1.20 (BLOOD.EXE from 3DFX patch, NOT 3DFX.EXE)
* Plasma Pak 1.10
* Plasma Pak 1.11 (19 August 1997)
* Plasma Pak 1.11 (23 September 1997)
* Plasma Pak 1.20 (BLOOD.EXE from 3DFX patch, NOT 3DFX.EXE)
* Plasma Pak 1.21 (One Unit Whole Blood)

## Building

Watcom 10.6 and TASM 3.1 are required to build.

### Build instructions
1) Build helix32 (i.e. `cd helix32` and then `wmake`)
2) Build qtools (i.e `cd qtools`, then `dobuild` and then select revision)
2) Build blood (i.e. `cd blood`, then `dobuild` and then select revision)

Special thanks to NY00123, Hendricks266, sirlemonhead, tmyqlfpir and Maxi Clouds.

