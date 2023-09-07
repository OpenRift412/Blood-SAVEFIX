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
Blood (1997) source code reconstruction by nukeykt.

Watcom 10.6 and TASM 3.2 are required to build.

## Build instructions
1) Build helix32 and qtools (e.g. `cd helix32` and then `wmake`)
2) Build blood (e.g. `cd blood` and then `wmake`)

Special thanks to NY00123, Hendricks266, sirlemonhead and tmyqlfpir.
