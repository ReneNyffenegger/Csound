//  Copyright (C) 2002-2004 Gabriel Maldonado
//
//  The gab library is free software; you can redistribute it
//  and/or modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  The gab library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with the gab library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
//  02111-1307 USA

// Ported to csound5 by:Andres Cabrera andres@geminiflux.com


#ifndef GAB_H
#define GAB_H

typedef	struct {
	OPDS	h;
	MYFLT	*ar, *asig, *kcf, *kbw, *ord, *iscl, *istor;
	int     scale, loop;
	MYFLT	c1, c2, c3, *yt1, *yt2, cosf, prvcf, prvbw;
	AUXCH	aux;
} KRESONX;

typedef struct {
	OPDS	h;
	MYFLT	*sr, *xcps, *iphs;
	double	curphs;
	int flag;
} METRO;

typedef struct {
	OPDS	h;
	MYFLT	*rslt, *xndx, *xfn, *ixmode;
	MYFLT   *table;
	MYFLT  	xbmul;
	int		xmode;
	//FUNC	*ftp;
} FASTAB;

typedef struct {
	OPDS	h;
	MYFLT	*r, *ndx;
} FASTB;

typedef struct {
	OPDS	h;
	MYFLT	*ifn;
} TB_INIT;


// ************************************************************ */
// Opcodes from Peter Neub�ker                                  */
// ************************************************************ */
// typedef struct {
//     OPDS    h;
//     MYFLT   *ifilcod;
// } PRINTI; 

//======================
//opcodes from Jens Groh
//======================
typedef struct { /* for nlalp opcode */
   OPDS h; /* header */
   MYFLT *aresult; /* resulting signal */
   MYFLT *ainsig; /* input signal */
   MYFLT *klfact; /* linear factor */
   MYFLT *knfact; /* nonlinear factor */
   MYFLT *istor; /* initial storage disposition */
   double m0; /* energy storage */
   double m1; /* energy storage */
} NLALP; 

//end opcodes from Jens Groh

typedef struct {
    OPDS    h;
    MYFLT   *sr, *kamp, *kcps, *ifn, *ifreqtbl, *iamptbl, *icnt, *iphs;
    FUNC    *ftp;
    FUNC    *freqtp;
    FUNC    *amptp;
    int     count;
    int     inerr;
    AUXCH   lphs;
	MYFLT	*previousAmp;
} ADSYNT2;

typedef struct {
    OPDS    h;
} EXITNOW;

typedef struct	{
	OPDS	h;
	MYFLT	*trig, *ndx, *maxtics, *ifn, *outargs[VARGMAX];
	int		numouts, currtic, old_ndx;
	MYFLT *table;
} SPLIT_TRIG;


typedef struct	{
	OPDS	h;
	MYFLT	*ktrig, *kphs, *ifn, *args[VARGMAX];
	MYFLT endSeq, *table, oldPhs;
	int numParm, /*currIndex,*/ endIndex, prevIndex, nextIndex /*, lastIndex */;
	MYFLT prevActime, nextActime/*, lastActime*/;
	int initFlag/*,jump*/;

} TIMEDSEQ;

#include "H/ugrw1.h"  //for zread function

#include "ugens6.h"  //for a_k_set function

typedef struct	{
	OPDS	h;
	MYFLT	*ktrig_start, *ktrig_stop, *numtics, *kfn, *inargs[VARGMAX];
	int	    recording, numins; 
	long	currtic, ndx, tablen;
	MYFLT	*table, old_fn;
	
} TABREC;

typedef struct	{
	OPDS	h;
	MYFLT	*ktrig, *numtics, *kfn, *outargs[VARGMAX];
	int	    playing, numouts; 
	long	currtic, ndx, tablen;
	MYFLT	*table, old_fn;
} TABPLAY;

typedef struct	{
	OPDS	h;
	MYFLT	*ktrig, *inargs[VARGMAX];
	MYFLT	old_inargs[VARGMAX];
	int numargs;
} ISCHANGED;

typedef struct	{
	OPDS	h;
	MYFLT	*commandLine;
} CSSYSTEM;

typedef struct	{
	OPDS	h;
	MYFLT	*kout, *asig, *ktrig, *imaxflag;
	MYFLT	max;
	int		counter;
} P_MAXIMUM;

//From fractals.h
typedef struct	{
	OPDS	h;
	MYFLT	*kr, *koutrig,  *ktrig, *kx, *ky, *kmaxIter;
	MYFLT oldx, oldy;
	int oldCount;
} MANDEL;

#endif
