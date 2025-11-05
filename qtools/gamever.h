// *** VERSIONS RESTORATION ***
#ifndef QTGAMEVER_H
#define QTGAMEVER_H

// It is assumed here that:
// 1. The compiler is set up to appropriately define APPVER_EXEDEF
// as an EXE identifier.
// 2. This header is included (near) the beginning of every compilation unit,
// in order to have an impact in any place where it's expected to.

// APPVER_QTOOLSREV definitions
#define AV_QR_QT110 199708120 // 12 August 1997
#define AV_QR_QT111A 199709230 // 23 September 1997

// Now define APPVER_QTOOLSREV to one of the above, based on APPVER_EXEDEF

#define APPVER_CONCAT1(x,y) x ## y
#define APPVER_CONCAT2(x,y) APPVER_CONCAT1(x,y)
#define APPVER_QTOOLSREV APPVER_CONCAT2(AV_QR_,APPVER_EXEDEF)

#endif // QTGAMEVER_H
