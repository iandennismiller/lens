#include <string.h>
#include "system.h"
#include "util.h"
#include "type.h"
#include "network.h"
#include "connect.h"
#include "example.h"
#include "train.h"
#include "act.h"
#include "display.h"
#include "command.h"
#include "control.h"
#include "object.h"

/* Add your initials or something here to customize the version number */
char *VersionExt = "-dcp";

/******************************** Extension Records **************************/

ObjInfo NetExtInfo;
ObjInfo GroupExtInfo;
ObjInfo UnitExtInfo;
ObjInfo BlockExtInfo;
ObjInfo ExSetExtInfo;
ObjInfo ExampleExtInfo;
ObjInfo EventExtInfo;

/* This is what the object viewer displays as the name of the extension. */
static void extensionName(void *ext, char *dest) {
  if (ext) sprintf(dest, "extension");
  else dest[0] = '\0';
}

struct netExt {
  int	numFrontEvents;
  flag	inFront;
};

/* This is how you register fields so they are visible in the object viewer 
   and accessible with setObject and getObject */
static void initNetExtInfo(void) {
  NetExt NX;
  addObj(NetExtInfo, "numFrontEvents", OFFSET(NX, numFrontEvents), TRUE, IntInfo);
  addObj(NetExtInfo, "inFront", OFFSET(NX, inFront), TRUE, IntInfo);
}

/* Use this to build the network extension and set your own default 
   parameters */
flag initNetworkExtension(Network N) {
  FREE(N->ext);

  N->ext = safeCalloc(1, sizeof(struct netExt), 
     "initNetworkExtension:N->ext");
  N->ext->numFrontEvents = 0;
  N->ext->inFront = 0;

  return TCL_OK;
}

/* This is called when the network is reset.  If you also want to reset group
   or other extensions, you should do that from within here. */
flag resetNetworkExtension(Network N) {
  N->ext->numFrontEvents = 0;
  N->ext->inFront = 0;
  return TCL_OK;
}

flag freeNetworkExtension(Network N) {
  FREE(N->ext);
  return TCL_OK;
}

flag copyNetworkExtension(Network from, Network to) {
  if (!from->ext) {
    if (to->ext) freeNetworkExtension(to);
  } else {
    if (!to->ext) initNetworkExtension(to);
    memcpy(to->ext, from->ext, sizeof(struct netExt));
  }
  return TCL_OK;
}


struct groupExt {
  flag	frontSide, backSide;
  int   runOnce;
  real  maxOut ;
  real  secondOut ;
  real  thirdOut ;
  real  maxOutSum ;
  real  secondOutSum ;
  real  thirdOutSum ;
  int	maxOutNum ;
  int	maxTargNum ;
  int	RT ;
};

static void initGroupExtInfo(void) {
  GroupExt GE;
  addObj(GroupExtInfo, "frontSide", OFFSET(GE, frontSide), TRUE, FlagInfo);
  addObj(GroupExtInfo, "backSide", OFFSET(GE, backSide), TRUE, FlagInfo);
  addObj(GroupExtInfo, "runOnce", OFFSET(GE, runOnce), TRUE, IntInfo);
  addObj(GroupExtInfo, "maxOut", OFFSET(GE, maxOut), TRUE, RealInfo);
  addObj(GroupExtInfo, "secondOut", OFFSET(GE, secondOut), TRUE, RealInfo);
  addObj(GroupExtInfo, "thirdOut", OFFSET(GE, thirdOut), TRUE, RealInfo);
  addObj(GroupExtInfo, "maxOutSum", OFFSET(GE, maxOutSum), TRUE, RealInfo);
  addObj(GroupExtInfo, "secondOutSum", OFFSET(GE, secondOutSum), TRUE, RealInfo);
  addObj(GroupExtInfo, "thirdOutSum", OFFSET(GE, thirdOutSum), TRUE, RealInfo);
  addObj(GroupExtInfo, "maxOutNum", OFFSET(GE, maxOutNum), TRUE, IntInfo);
  addObj(GroupExtInfo, "maxTargNum", OFFSET(GE, maxTargNum), TRUE, IntInfo);
  addObj(GroupExtInfo, "RT", OFFSET(GE, RT), TRUE, IntInfo);
}

flag initGroupExtension(Group G) {
  FREE(G->ext);

  G->ext = safeCalloc(1, sizeof(struct groupExt), 
     "initGroupExtension:G->ext");
  G->ext->frontSide = 0;
  G->ext->backSide = 0;
  G->ext->runOnce = 0 ;
  G->ext->maxOut     = 0.0 ;
  G->ext->secondOut  = 0.0 ;
  G->ext->thirdOut   = 0.0 ;
  G->ext->maxOutSum     = 0.0 ;
  G->ext->secondOutSum  = 0.0 ;
  G->ext->thirdOutSum   = 0.0 ;
  G->ext->maxOutNum  = -1 ;
  G->ext->maxTargNum = -1 ;
  G->ext->RT = -1 ;
  return TCL_OK;
}

flag freeGroupExtension(Group G) {
  FREE(G->ext);
  return TCL_OK;
}

flag copyGroupExtension(Group from, Group to) {
  if (!from->ext) {
    if (to->ext) freeGroupExtension(to);
  } else {
    if (!to->ext) initGroupExtension(to);
    memcpy(to->ext, from->ext, sizeof(struct groupExt));
  }
  return TCL_OK;
}


struct unitExt {
  real row;
  real col;
  real outWtSum ;
  int  excludeLimOutWts ;
};

static void initUnitExtInfo(void) {
  UnitExt UE;
  addObj(UnitExtInfo, "row", OFFSET(UE, row), TRUE, RealInfo);
  addObj(UnitExtInfo, "col", OFFSET(UE, col), TRUE, RealInfo);
  addObj(UnitExtInfo, "outWtSum", OFFSET(UE, outWtSum), TRUE, RealInfo);
  addObj(UnitExtInfo, "excludeLimOutWts", OFFSET(UE, excludeLimOutWts), TRUE, IntInfo);
}

flag initUnitExtension(Unit U) {
  FREE(U->ext);
  U->ext = safeCalloc(1, sizeof(struct unitExt), 
     "initUnitExtension:U->ext");
  U->ext->row = -1.0 ;
  U->ext->col = -1.0 ;
  U->ext->outWtSum     = -1.0 ;
  U->ext->excludeLimOutWts = 0 ;
  return TCL_OK;
}

flag freeUnitExtension(Unit U) {
  FREE(U->ext);
  return TCL_OK;
}

flag copyUnitExtension(Unit from, Unit to) {
  if (!from->ext) {
    if (to->ext) freeUnitExtension(to);
  } else {
    if (!to->ext) initUnitExtension(to);
    memcpy(to->ext, from->ext, sizeof(struct unitExt));
  }
  return TCL_OK;
}


struct blockExt {
  int removeMe;
};

static void initBlockExtInfo(void) {
  /* BlockExt BE; */
}

flag initBlockExtension(Block B) {
  FREE(B->ext);
  return TCL_OK;
}

flag freeBlockExtension(Block B) {
  FREE(B->ext);
  return TCL_OK;
}

flag copyBlockExtension(Block from, Block to) {
  if (!from->ext) {
    if (to->ext) freeBlockExtension(to);
  } else {
    if (!to->ext) initBlockExtension(to);
    memcpy(to->ext, from->ext, sizeof(struct blockExt));
  }
  return TCL_OK;
}


struct exSetExt {
  int removeMe;
};

static void initExSetExtInfo(void) {
  /* ExSetExt SE; */
}

flag initExSetExtension(ExampleSet S) {
  FREE(S->ext);
  return TCL_OK;
}

flag freeExSetExtension(ExampleSet S) {
  FREE(S->ext);
  return TCL_OK;
}

flag copyExSetExtension(ExampleSet from, ExampleSet to) {
  if (!from->ext) {
    if (to->ext) freeExSetExtension(to);
  } else {
    if (!to->ext) initExSetExtension(to);
    memcpy(to->ext, from->ext, sizeof(struct groupExt));
  }
  return TCL_OK;
}


struct exampleExt {
  int removeMe;
};

static void initExampleExtInfo(void) {
  /* ExampleExt EE; */
}

flag initExampleExtension(Example E) {
  FREE(E->ext);
  return TCL_OK;
}

flag freeExampleExtension(Example E) {
  FREE(E->ext);
  return TCL_OK;
}

flag copyExampleExtension(Example from, Example to) {
  if (!from->ext) {
    if (to->ext) freeExampleExtension(to);
  } else {
    if (!to->ext) initExampleExtension(to);
    memcpy(to->ext, from->ext, sizeof(struct exampleExt));
  }
  return TCL_OK;
}


struct eventExt {
  int removeMe;
};

static void initEventExtInfo(void) {
  /* EventExt VE; */
}

flag initEventExtension(Event V) {
  FREE(V->ext);
  return TCL_OK;
}

flag freeEventExtension(Event V) {
  FREE(V->ext);
  return TCL_OK;
}

flag copyEventExtension(Event from, Event to) {
  if (!from->ext) {
    if (to->ext) freeEventExtension(to);
  } else {
    if (!to->ext) initEventExtension(to);
    memcpy(to->ext, from->ext, sizeof(struct eventExt));
  }
  return TCL_OK;
}


/********************************* New Tcl Commands **************************/
/* Here is a sample Tcl-callable C stub.  util.c contains the functions
   warning(), result(), and append() for creating Tcl return messages.
   The stub must be registered (see below) before it can be called. */
/*
static int C_foo(TCL_CMDARGS) {
  char *usage = "foo <bar>";
  if (argc == 2 && !strcmp(argv[1], "-h"))
    return commandHelp(argv[0]);
  if (argc != 2) return usageError(argv[0], usage);
  if (!Net) return warning("%s: no current network", argv[0]);
  
  return TCL_OK;
}
*/

/////////////////////////////////////////////////////////////////////////////
// "run-once" groups in continuous networks

static void dotProductOnceInput(Group G, GroupProc P) {

  if (G->ext->runOnce && G->net->currentTick != G->ext->runOnce)
    return ;

  FOR_EACH_UNIT(G, {
    real input = 0.0; 
    FOR_EACH_LINK_FAST_FORW(U, input += V_OUT * L_WGT, {
      input +=  V_OUT * L_WGT + V_OUT_(1) * L_WGT_(1) +
	V_OUT_(2) * L_WGT_(2) + V_OUT_(3) * L_WGT_(3) +
	V_OUT_(4) * L_WGT_(4) + V_OUT_(5) * L_WGT_(5) +
	V_OUT_(6) * L_WGT_(6) + V_OUT_(7) * L_WGT_(7) +
	V_OUT_(8) * L_WGT_(8) + V_OUT_(9) * L_WGT_(9);
    });
    U->input = input;
  });
}

static void dotProductOnceInputBack(Group G, GroupProc P) {

  if (G->ext->runOnce && G->net->currentTick != G->ext->runOnce) return ;

  FOR_EACH_UNIT(G, {
    real inputDeriv = U->inputDeriv;
    FOR_EACH_LINK_FAST_BACK(U, {
      V_DRV     += inputDeriv * L_WGT;
      L_DRV     += inputDeriv * V_OUT;
    }, {
      V_DRV_(0) += inputDeriv * L_WGT_(0);
      L_DRV_(0) += inputDeriv * V_OUT_(0);
      V_DRV_(1) += inputDeriv * L_WGT_(1);
      L_DRV_(1) += inputDeriv * V_OUT_(1);
      V_DRV_(2) += inputDeriv * L_WGT_(2);
      L_DRV_(2) += inputDeriv * V_OUT_(2);
      V_DRV_(3) += inputDeriv * L_WGT_(3);
      L_DRV_(3) += inputDeriv * V_OUT_(3);
      V_DRV_(4) += inputDeriv * L_WGT_(4);
      L_DRV_(4) += inputDeriv * V_OUT_(4);
      V_DRV_(5) += inputDeriv * L_WGT_(5);
      L_DRV_(5) += inputDeriv * V_OUT_(5);
      V_DRV_(6) += inputDeriv * L_WGT_(6);
      L_DRV_(6) += inputDeriv * V_OUT_(6);
      V_DRV_(7) += inputDeriv * L_WGT_(7);
      L_DRV_(7) += inputDeriv * V_OUT_(7);
      V_DRV_(8) += inputDeriv * L_WGT_(8);
      L_DRV_(8) += inputDeriv * V_OUT_(8);
      V_DRV_(9) += inputDeriv * L_WGT_(9);
      L_DRV_(9) += inputDeriv * V_OUT_(9);
    });
  });
}

static void dotProductOnceInputInit(Group G, GroupProc P) {
  P->forwardProc  = dotProductOnceInput;
  P->backwardProc = dotProductOnceInputBack;
}


static void logisticOnceOutput(Group G, GroupProc P) {
  if (G->ext->runOnce && G->net->currentTick != G->ext->runOnce) return ;

  if (G->type & ADAPTIVE_GAIN) {
    FOR_EACH_UNIT(G, {
      U->output = fastSigmoid(U->input * U->gain);
    });
  } else {
    real gain = chooseValue(G->gain, Net->gain);
    FOR_EACH_UNIT(G, U->output = fastSigmoid(U->input * gain));
  }
}

static void logisticOnceOutputBack(Group G, GroupProc P) {

  if (G->ext->runOnce && G->net->currentTick != G->ext->runOnce) return ;

  if (G->type & ADAPTIVE_GAIN) {
    real x;
    FOR_EACH_UNIT(G, {
      x = U->outputDeriv * U->output * (1.0 - U->output);
      U->inputDeriv = x * U->gain;
      U->gainDeriv += x * U->input;
    });
  } else {    
    real gain = chooseValue(G->gain, Net->gain);
    FOR_EACH_UNIT(G, U->inputDeriv = U->outputDeriv * 
		  U->output * (1.0 - U->output) * gain);
  }
}

static void logisticOnceOutputInit(Group G, GroupProc P) {
  P->forwardProc  = logisticOnceOutput;
  P->backwardProc = logisticOnceOutputBack;
}


/////////////////////////////////////////////////////////////////////////////
// topo nets

static int C_topoInitPositions(TCL_CMDARGS) {
  char *usage = "topoInitPositions <group-list>";
  if (argc == 2 && !strcmp(argv[1], "-h")) 
    return commandHelp(argv[0]);
  if (argc != 2) return usageError(argv[0], usage);
  if (!Net) return warning("%s: no current network", argv[0]);

  FOR_EACH_GROUP_IN_LIST(argv[1], {
    FOR_EACH_UNIT(G, {
	U->ext->row = (real)(U->plotRow) ;
	U->ext->col = (real)(U->plotCol) ;
	}) ;
    });
  return TCL_OK;
}

static int C_topoSetPosition(TCL_CMDARGS) {
  char *usage = "topoSetPosition <group-list> <row> <col>";
  int newRow, newCol ;
  if (argc == 2 && !strcmp(argv[1], "-h")) 
    return commandHelp(argv[0]);
  if (argc != 4) return usageError(argv[0], usage);
  if (!Net) return warning("%s: no current network", argv[0]);
  newRow = ator(argv[2]) ; 
  newCol = ator(argv[3]) ;

  FOR_EACH_GROUP_IN_LIST(argv[1], {
      FOR_EACH_UNIT(G, {
	  U->ext->row = newRow ;
	  U->ext->col = newCol ;
	}) ;
    });
  return TCL_OK;
}

static int C_topoSetRow(TCL_CMDARGS) {
  char *usage = "topoSetPosition <group-list> <row>";
  int newRow ;
  if (argc == 2 && !strcmp(argv[1], "-h")) 
    return commandHelp(argv[0]);
  if (argc != 3) return usageError(argv[0], usage);
  if (!Net) return warning("%s: no current network", argv[0]);
  newRow = ator(argv[2]) ; 

  FOR_EACH_GROUP_IN_LIST(argv[1], {
      FOR_EACH_UNIT(G, {
	  U->ext->row = newRow ;
	}) ;
    });
  return TCL_OK;
}

static int C_topoSetCol(TCL_CMDARGS) {
  char *usage = "topoSetPosition <group-list> <col>";
  int newCol ;
  if (argc == 2 && !strcmp(argv[1], "-h")) 
    return commandHelp(argv[0]);
  if (argc != 3) return usageError(argv[0], usage);
  if (!Net) return warning("%s: no current network", argv[0]);
  newCol = ator(argv[2]) ; 

  FOR_EACH_GROUP_IN_LIST(argv[1], {
      FOR_EACH_UNIT(G, {
	  U->ext->col = newCol ;
	}) ;
    });
  return TCL_OK;
}

static int C_topoScaleDerivs(TCL_CMDARGS) {
  char *usage = "topoScaleDerivs <sd>";
  if (argc == 2 && !strcmp(argv[1], "-h")) 
    return commandHelp(argv[0]);
  if (argc != 2) return usageError(argv[0], usage);
  if (!Net) return warning("%s: no current network", argv[0]);
  FOR_EACH_GROUP({
      real sd = ator(argv[1]) ;
      FOR_EACH_UNIT(G, {
	if (U->ext->row >= 0.0 && U->ext->col >= 0.0) {
	  FOR_EACH_LINK_AND_SENDING_UNIT(U, {
	      if (V->ext->row >= 0.0 && V->ext->col >= 0.0) {
		real d = DIST(U->ext->row, U->ext->col, V->ext->row, V->ext->col) ;
		L->deriv *= GAUSS(d,sd) ;
	      }
	    }) ;
	}
	}) ;
    });
  return TCL_OK;
}

static int C_topoWeightDecay(TCL_CMDARGS) {
  char *usage = "topoWeightDecay <sd> <decay>";
  if (argc == 2 && !strcmp(argv[1], "-h")) 
    return commandHelp(argv[0]);
  if (argc != 3) return usageError(argv[0], usage);
  if (!Net) return warning("%s: no current network", argv[0]);

  FOR_EACH_GROUP({
      real sd    = ator(argv[1]) ; 
      real decay = ator(argv[2]) ;
      FOR_EACH_UNIT(G, {
	if (U->ext->row >= 0 && U->ext->col >= 0) {
	  FOR_EACH_LINK_AND_SENDING_UNIT(U, {
	      if (V->ext->row >= 0 && V->ext->col >= 0) {
		real d = DIST(U->ext->row, U->ext->col, V->ext->row, V->ext->col) ;
		L->deriv += decay * L->weight * (1.0 - GAUSS(d,sd)) ;
	      }
	    }) ;
	}
	}) ;
    });
  return TCL_OK;
}

static int C_limitOutWtMag(TCL_CMDARGS) {
  char *usage = "limitOutWtMag <group-list> <limit> [-exclude <receiving group-list>]";
  real limit ;
  if (argc == 2 && !strcmp(argv[1], "-h")) 
    return commandHelp(argv[0]);
  if (argc != 3 && argc != 5) return usageError(argv[0], usage);
  if (!Net) return warning("%s: no current network", argv[0]);
  limit = ator(argv[2]) ;

  // set outWtSum to 0 for specified groups; -1 for everyone else
  FOR_EACH_GROUP({ 
      FOR_EACH_UNIT(G, {
	  U->ext->outWtSum = -1.0 ;
	  U->ext->excludeLimOutWts = 0 ;
	}) ;
    }) ;
  FOR_EACH_GROUP_IN_LIST(argv[1], {
      FOR_EACH_UNIT(G, {
	  U->ext->outWtSum =  0.0 ;
	}) ;
    }) ;

  // maybe exclude some receiving groups
  if (argc > 3 && !strcmp(argv[3], "-exclude"))
    FOR_EACH_GROUP_IN_LIST(argv[4], {
	FOR_EACH_UNIT(G, {
	    U->ext->excludeLimOutWts =  1 ;
	  }) ;
      }) ;

  // compute sum of absolute magnitude of outgoing wts for all specified groups
  FOR_EACH_GROUP({
    FOR_EACH_UNIT(G, {
      FOR_EACH_LINK_AND_SENDING_UNIT(U, {
	  if (V->ext->outWtSum >= 0.0 && !U->ext->excludeLimOutWts) 
	    V->ext->outWtSum += fabs(L->weight) ;
	}) ;
      }) ;
    });

  // compute scale factor for units whose sum exceeds limit (otherwise set to -1)
  FOR_EACH_GROUP({
    FOR_EACH_UNIT(G, {
	U->ext->outWtSum = (U->ext->outWtSum > limit ? limit / U->ext->outWtSum : -1.0) ;
      }) ;
    });

  // scale weights of links whose sending units have specified scale factors (not -1)
  FOR_EACH_GROUP({
    FOR_EACH_UNIT(G, {
      FOR_EACH_LINK_AND_SENDING_UNIT(U, {
	  if (V->ext->outWtSum > 0.0 && !U->ext->excludeLimOutWts) 
	    L->weight *= V->ext->outWtSum ;
	}) ;
      }) ;
    });
  return TCL_OK;
}



/////////////////////////////////////////////////////////////////////////////
// decoding PMSP phonology

#define NSYL_RLEN		1024	/* maximum response length */
#define	NSYL_N_PHONEMES		61
#define NSYL_PHON_VOWEL_SLOT	3
#define NSYL_N_PHON_SLOTS	13
#define NSYL_N_PHON_ONSETS	23
#define	NSYL_N_PHON_VOWELS	14
#define	NSYL_ON_THRESHOLD	0.5

static char	*nsyllable_phon_slots[NSYL_N_PHON_SLOTS] = { 
"sSC", "zZjfvTDpbtdkgmnh", "lrwy",
"aeiou@^AEIOUWY",
"r", "l", "mnN", "bgd", "PK+", "sz", "fvpk", "t", "SZTDCj"
};


static int C_decodePMSP(TCL_CMDARGS) {
  char *usage = "decodePMSP <phonGroup>";
  Group phonGroup ;
  Unit  unit ;
  int slot ;
  char c=(char)NULL, *s, response[NSYL_RLEN], affricate=(char)NULL ;

  if (argc ==2 && !strcmp(argv[1], "-h"))
    return commandHelp(argv[0]);
  if (argc != 2) return usageError(argv[0], usage);
  if (!Net) return warning("%s: no current network", argv[0]);

  if (!(phonGroup = lookupGroup(argv[1])))
    return warning("%s: group \"%s\" doesn't exist", argv[0], argv[1]);
  
  for (slot = 0, unit = phonGroup->unit, response[0]=(char)NULL ; slot < NSYL_N_PHON_SLOTS ; unit += strlen(nsyllable_phon_slots[slot]), slot++) {
    Unit	slotUnit ;
    real	wtaOutput=-1.0 ;

    // FIND MOST ACTIVE UNIT IN SLOT
    for (s = nsyllable_phon_slots[slot], slotUnit = unit ; *s ; s++, slotUnit++) {
      if (slotUnit->output > wtaOutput) { 
	wtaOutput = slotUnit->output ; c = *s ; 
      }
    }

    // ADD TO RESPONSE IF VOWEL OR ABOVE ON_THRESHOLD
    if (slot == NSYL_PHON_VOWEL_SLOT || (wtaOutput >= NSYL_ON_THRESHOLD)) {
      if (strchr("PK+", c)) {	// AFFRICATE
	affricate = (c=='P'?'p':(c=='K'?'k':'t')) ;
	sprintf(response, "%s%cs", response, affricate) ;
      } else if (!affricate || (c!=affricate && c!='s')) {
	sprintf(response, "%s%c", response, c) ;
      }
    }
  } //slot
  
  printf("%s\n", response) ;
  return TCL_OK;
}


/////////////////////////////////////////////////////////////////////////////
// group stopping criteria

static real summedOutput (Unit U) {
  int tick = 0, lastTick = U->group->net->currentTick ;
  real sum = 0.0 ;
  for (tick = 0 ; tick <= lastTick ; tick++)
    sum += U->outputHistory[HISTORY_INDEX(tick)] ;
  return sum ;
}


static flag gapGroupCriterion(Group G, real criterion) {
  Unit max = NULL, second = NULL, third = NULL, targ = NULL ;
  FOR_EACH_UNIT(G, {
    if (isNaN(U->target)) continue;
    if      (!max || U->output > max->output)    {third = second; second = max; max = U;}
    else if (!second || U->output > second->output) {third = second; second = U;}
    else if (!third || U->output > third->output)  {third = U;}
    if (!targ || U->target > targ->target) {targ = U;}
  });
  G->ext->maxOut     = max->output ;
  G->ext->secondOut  = second->output ;
  G->ext->thirdOut   = third->output ;
  G->ext->maxOutNum  = max->num ;
  G->ext->maxTargNum = targ->num ;
  G->ext->RT         = G->net->currentTick ;
  if (max->num >= 0 && ABS(max->output - second->output) > criterion) {
    G->ext->maxOutSum    = summedOutput(max) ;
    G->ext->secondOutSum = summedOutput(second) ;
    G->ext->thirdOutSum  = summedOutput(third) ;
    return TRUE;
  }
  return FALSE ;
}

static void gapGroupCriterionInit(Group G, GroupProc P) {
  G->groupCriterionReached = gapGroupCriterion;
}


/* The most active unit must exceed the criterion */
static flag fixedGroupCriterion(Group G, real criterion) {
 FOR_EACH_UNIT(G, {
   if (isNaN(U->target)) continue;
   if ((U->target > criterion && U->output <= criterion) ||
   (U->target < criterion && U->output >= criterion)) return FALSE ;
   });
 return TRUE;
}

static void fixedGroupCriterionInit(Group G, GroupProc P) {
  G->groupCriterionReached = fixedGroupCriterion;
}


/* maximum change in activation must fall below criterion */
static flag maxChangeGroupCriterion(Group G, real criterion) {
  int lastTick = G->net->currentTick - 1 ;
  FOR_EACH_UNIT(G, {
    if (fabs(U->output - U->outputHistory[lastTick]) >= criterion)
      return FALSE ;
  });
  return TRUE;
}

static void maxChangeGroupCriterionInit(Group G, GroupProc P) {
  G->groupCriterionReached = maxChangeGroupCriterion;
}


/* average change in activation must fall below criterion */
static flag aveChangeGroupCriterion(Group G, real criterion) {
  int lastTick = G->net->currentTick - 1 ;
  real sum = 0.0 ;
  FOR_EACH_UNIT(G, {
    sum += fabs(U->output - U->outputHistory[lastTick]) ;
  });
  if (sum / (float)G->numUnits < criterion)
    return TRUE ;
  return FALSE;
}

static void aveChangeGroupCriterionInit(Group G, GroupProc P) {
  G->groupCriterionReached = aveChangeGroupCriterion;
}


/////////////////////////////////////////////////////////////////////////////
// autosequencers

flag sequenceNetForward(Event V) {
  flag inFront = Net->ext->inFront;
  
  FOR_EACH_GROUP(if ((inFront && G->ext->frontSide)||(!inFront && !G->ext->frontSide)) {
    computeInput(G,  TRUE);
    computeOutput(G, TRUE);
  } else {
    resetOutputs(G);
    storeOutputs(G,Net->currentTick);
    computeCost(G, TRUE);
  });
  return TCL_OK;
}

flag sequenceNetBackward(Event V) {
  int i, lastSource = -1, origTick;
  flag inFront = Net->ext->inFront;

  FOR_EACH_GROUP( if ((inFront && G->ext->frontSide)||(!inFront && G->ext->backSide)) {
    restoreInputs(G, Net->currentTick);
    restoreOutputs(G, Net->currentTick);
    if( !(G->ext->frontSide && G->ext->backSide) ) {
      resetOutputDerivs(G);
    }
  });
  RUN_PROC(preTickBackProc);
  FOR_EACH_GROUP_BACK(if ((inFront && G->ext->frontSide)||(!inFront && G->ext->backSide)) {
    BACKPROP(G)
  });
  
  /* This will extend the backprop backward in time, which may be used in 
     simple recurrent networks.  lastSource is the last group that is an 
     Elman layer or a source for one.  This is used to avoid unnecessary 
     work. */
  if (Net->backpropTicks > 1) {
    GroupProc P;
    flag wasReset;
    /* Find the last group that we need to consider. */
    FOR_EACH_GROUP(if ((inFront && G->ext->frontSide)||(!inFront && G->ext->backSide)) {
      if (G->type & ELMAN) {
	if (g > lastSource) lastSource = g;
	for (P = G->outputProcs; P; P = P->next)
	  if (P->type == ELMAN_CLAMP && ((Group) P->otherData) && 
	      ((Group) P->otherData)->num > lastSource)
	    lastSource = ((Group) P->otherData)->num;
      }});
    
    origTick = Net->currentTick;

    for (i = 1; i < Net->backpropTicks; i++) {
      wasReset = GET_HISTORY(Net,resetHistory,HISTORY_INDEX(Net->currentTick));
      Net->currentTick--;
      if( (inFront && Net->currentTick < 0) || (!inFront && Net->currentTick < Net->ext->numFrontEvents) ) break; 
      /* Set the outputs to the way they were at the end of t-i. */
      FOR_EACH_GROUP_IN_RANGE_BACK(lastSource, 0, if ((inFront && G->ext->frontSide)||(!inFront && G->ext->backSide)) {
	restoreOutputs(G, Net->currentTick);
	restoreInputs(G, Net->currentTick);
	if (wasReset && G->type & RESET_ON_EXAMPLE) {
	  resetOutputDerivCache(G);
	  resetBackwardIntegrators(G);
	}
      });
      /* Backpropagate. */
      FOR_EACH_GROUP_IN_RANGE_BACK(lastSource, 0, if ((inFront && G->ext->frontSide)||(!inFront && G->ext->backSide)) {
	FOR_EACH_UNIT(G, U->outputDeriv = 0.0);
	BACKPROP(G);
      });
    }

    /* Restore the network to the original state. */
    Net->currentTick = origTick;
    FOR_EACH_GROUP_IN_RANGE(0, lastSource, if ((inFront && G->ext->frontSide)||(!inFront && G->ext->backSide)) {
      restoreOutputs(G, Net->currentTick);
    });
  }
  return TCL_OK;
}

flag sequenceNetForwardBackward(Event V) {
  int i, lastSource = -1, origTick;
  flag inFront = Net->ext->inFront;
  
  FOR_EACH_GROUP( if ((inFront && G->ext->frontSide)||(!inFront && G->ext->backSide)) {
    if( G->ext->frontSide && G->ext->backSide ) {
      restoreInputs(G, Net->ext->numFrontEvents-1 );
      restoreOutputs(G, Net->ext->numFrontEvents-1 );
      storeInputs(G, Net->currentTick);
      storeOutputs(G, Net->currentTick);
      cacheOutputDerivs(G);
    } else {
      computeInput(G,  TRUE);
      computeOutput(G, TRUE);
      resetOutputDerivCache(G);
    }
    computeCost(G,   TRUE);
    resetBackwardIntegrators(G);
    computeCostBack(G, TRUE);
  } else {
    resetOutputs(G);
    storeOutputs(G, Net->currentTick);
  });
  RUN_PROC(preTickBackProc);
  FOR_EACH_GROUP_BACK(if ((inFront && G->ext->frontSide)||(!inFront && G->ext->backSide)) {
    BACKPROP(G);
  });

  FOR_EACH_GROUP( if ( G->ext->frontSide && G->ext->backSide ) { injectOutputDerivCache(G); } );
  
  /* This will extend the backprop backward in time, which may be used in 
     simple recurrent networks.  lastSource is the last group that is an 
     Elman layer or a source for one.  This is used to avoid unnecessary 
     work. */
  if (Net->backpropTicks > 1) {
    GroupProc P;
    flag wasReset;
    /* Find the last group that we need to consider. */
    FOR_EACH_GROUP(if ((inFront && G->ext->frontSide)||(!inFront && G->ext->backSide)) {
      if (G->type & ELMAN) {
	if (g > lastSource) lastSource = g;
	for (P = G->outputProcs; P; P = P->next)
	  if (P->type == ELMAN_CLAMP && ((Group) P->otherData) && 
	      ((Group) P->otherData)->num > lastSource)
	    lastSource = ((Group) P->otherData)->num;
      }});
    
    origTick = Net->currentTick;

    for (i = 1; i < Net->backpropTicks; i++) {
      wasReset = GET_HISTORY(Net,resetHistory,HISTORY_INDEX(Net->currentTick));
      Net->currentTick--;
      if( (inFront && Net->currentTick < 0) || (!inFront && Net->currentTick < Net->ext->numFrontEvents) ) break; 
      /* Set the outputs to the way they were at the end of t-i. */
      FOR_EACH_GROUP_IN_RANGE_BACK(lastSource, 0, if ((inFront && G->ext->frontSide)||(!inFront && G->ext->backSide)) {
	restoreOutputs(G, Net->currentTick);
	restoreInputs(G, Net->currentTick);
	if (wasReset && G->type & RESET_ON_EXAMPLE) {
	  resetOutputDerivCache(G);
	  resetBackwardIntegrators(G);
	}
      });
      /* Backpropagate. */
      FOR_EACH_GROUP_IN_RANGE_BACK(lastSource, 0, if ((inFront && G->ext->frontSide)||(!inFront && G->ext->backSide)) {
	FOR_EACH_UNIT(G, U->outputDeriv = 0.0);
	BACKPROP(G);
      });
    }

    /* Restore the network to the original state. */
    Net->currentTick = origTick;
    FOR_EACH_GROUP_IN_RANGE(0, lastSource, if ((inFront && G->ext->frontSide)||(!inFront && G->ext->backSide)) {
      restoreOutputs(G, Net->currentTick);
    });
  }
  return TCL_OK;
}


flag sequenceNetRunExample(Example E, flag (*tickProc)(Event), flag *correct) {
  int tick = 0, ticksOnEvent, event = 0, 
    numFrontEvents = (Net->ext->numFrontEvents ? Net->ext->numFrontEvents : E->numEvents);
  flag eventDone, critReached, training = (tickProc == Net->netTrainTick);
  real timeOnEvent, minTime, maxTime, graceTime;
  Event V, origV;

  if (E->set->loadEvent(V = E->event)) return TCL_ERROR;
  origV = V;
  ticksOnEvent = 0;
  maxTime = chooseValue(V->maxTime, E->set->maxTime);
  minTime = chooseValue(V->minTime, E->set->minTime);
  graceTime = chooseValue(V->graceTime, E->set->graceTime);
  Net->inGracePeriod = (graceTime > 0.0);


  /* Reset the outputs and integrators if requested. */
  FOR_EACH_GROUP({
    resetOutputs(G);
    resetForwardIntegrators(G);
    resetOutputDerivs(G);
  });
  
  if (Net->type & CONTINUOUS) {
    storeOutputsAndTargets(0);
    Net->exampleHistoryStart = 0;
    Net->eventHistory[tick] = -1;
    tick = 1;
    updateDisplays(ON_TICK);
  } else {
    Net->exampleHistoryStart = 
      (Net->exampleHistoryStart + Net->ticksOnExample) % Net->historyLength;
  }
  Net->resetHistory[HISTORY_INDEX(0)] = TRUE;

  RUN_PROC(preEventProc);
  *correct = TRUE;

  /* first run the front side of the network only */
  /* this will generate a pattern on the middle level of rep */

  Net->ext->inFront = TRUE;
  for (; tick < Net->maxTicks && event < numFrontEvents; tick++) {
    Net->currentTick = tick;
    Net->eventHistory[tick] = event;
    if (tick > 0) Net->resetHistory[HISTORY_INDEX(tick)] = FALSE;
    
    RUN_PROC(preTickProc);
    if (sequenceNetForward(V)) return TCL_ERROR;   /* This is the important call */
    RUN_PROC(postTickProc);
    
    updateDisplays(ON_TICK);
    
    eventDone = FALSE;
    critReached = 2;
    ticksOnEvent++;
    /* The SMALL_VAL is to correct for possible floating point inaccuracy */
    timeOnEvent = ((real) ticksOnEvent / Net->ticksPerInterval) + SMALL_VAL;
    /* If we have done enough time on this event, go on */
    if (timeOnEvent >= maxTime || tick >= Net->maxTicks - 1)
      eventDone = TRUE;
    /* If we have gone long enough and reached criterion, go on */
    else if (timeOnEvent >= minTime && 
	     (critReached = groupCriteriaReached(training)))
      eventDone = TRUE;
    /* See if the grace period is over */
    else if (Net->inGracePeriod && timeOnEvent >= graceTime)
      Net->inGracePeriod = FALSE;

    if (eventDone) {
      if (critReached == 2) critReached = groupCriteriaReached(training);
      if (!critReached) *correct = FALSE;
      RUN_PROC(postEventProc);
      updateDisplays(ON_EVENT);

      if (Net->groupCritRequired && !critReached) break;

      if (++event < E->numEvents && ((V = E->event + event))) {
	if (E->set->loadEvent(V)) return TCL_ERROR;
	ticksOnEvent = 0;
	maxTime = chooseValue(V->maxTime, E->set->maxTime);
	minTime = chooseValue(V->minTime, E->set->minTime);
	graceTime = chooseValue(V->graceTime, E->set->graceTime);
	Net->inGracePeriod = (graceTime > 0.0);
	RUN_PROC(preEventProc);
      }
    }
  }

  /* next run the backside forward and backward, */
  /* using the middle pattern as an "input" layer */

  Net->ext->inFront = FALSE;
  ticksOnEvent = 0;
  for (; tick < Net->maxTicks && event < E->numEvents; tick++) {
    Net->currentTick = tick;
    Net->eventHistory[tick] = event;
    if (tick > 0) Net->resetHistory[HISTORY_INDEX(tick)] = FALSE;
    
    RUN_PROC(preTickProc);
    if (sequenceNetForwardBackward(V)) return TCL_ERROR;   /* This is the important call */
    RUN_PROC(postTickProc);
    
    updateDisplays(ON_TICK);
    
    eventDone = FALSE;
    critReached = 2;
    ticksOnEvent++;
    /* The SMALL_VAL is to correct for possible floating point inaccuracy */
    timeOnEvent = ((real) ticksOnEvent / Net->ticksPerInterval) + SMALL_VAL;
    /* If we have done enough time on this event, go on */
    if (timeOnEvent >= maxTime || tick >= Net->maxTicks - 1)
      eventDone = TRUE;
    /* If we have gone long enough and reached criterion, go on */
    else if (timeOnEvent >= minTime && 
	     (critReached = groupCriteriaReached(training)))
      eventDone = TRUE;
    /* See if the grace period is over */
    else if (Net->inGracePeriod && timeOnEvent >= graceTime)
      Net->inGracePeriod = FALSE;

    if (eventDone) {
      if (critReached == 2) critReached = groupCriteriaReached(training);
      if (!critReached) *correct = FALSE;
      RUN_PROC(postEventProc);
      updateDisplays(ON_EVENT);

      if (Net->groupCritRequired && !critReached) break;

      if (++event < E->numEvents && ((V = E->event + event))) {
	if (E->set->loadEvent(V)) return TCL_ERROR;
	ticksOnEvent = 0;
	maxTime = chooseValue(V->maxTime, E->set->maxTime);
	minTime = chooseValue(V->minTime, E->set->minTime);
	graceTime = chooseValue(V->graceTime, E->set->graceTime);
	Net->inGracePeriod = (graceTime > 0.0);
	RUN_PROC(preEventProc);
      }
    }
  }
  Net->ticksOnExample = Net->currentTick + 1;

  /* finally, backpropagate the frontside for the first N ticks */
  /* using the derivatives on the middle group as the error signal */

  Net->ext->inFront = TRUE;
  event = 0;
  V = origV;
  if (E->set->loadEvent(V)) return TCL_ERROR;
  ticksOnEvent = 0;
  maxTime = chooseValue(V->maxTime, E->set->maxTime);
  minTime = chooseValue(V->minTime, E->set->minTime);
  graceTime = chooseValue(V->graceTime, E->set->graceTime);
  Net->inGracePeriod = (graceTime > 0.0);

  for (tick = 0; tick < Net->maxTicks && event < numFrontEvents; tick++) {
    Net->currentTick = tick;
    // Net->eventHistory[tick] = event;
    // if (tick > 0) Net->resetHistory[HISTORY_INDEX(tick)] = FALSE;
    
    if (sequenceNetBackward(V)) return TCL_ERROR;   /* This is the important call */
    
    //    updateDisplays(ON_TICK);
    
    eventDone = FALSE;
    critReached = 2;
    ticksOnEvent++;
    /* The SMALL_VAL is to correct for possible floating point inaccuracy */
    timeOnEvent = ((real) ticksOnEvent / Net->ticksPerInterval) + SMALL_VAL;
    /* If we have done enough time on this event, go on */
    if (timeOnEvent >= maxTime || tick >= Net->maxTicks - 1)
      eventDone = TRUE;
    /* If we have gone long enough and reached criterion, go on */
    else if (timeOnEvent >= minTime && 
	     (critReached = groupCriteriaReached(training)))
      eventDone = TRUE;
    /* See if the grace period is over */
    else if (Net->inGracePeriod && timeOnEvent >= graceTime)
      Net->inGracePeriod = FALSE;

    if (eventDone) {
      if (critReached == 2) critReached = groupCriteriaReached(training);
      if (!critReached) *correct = FALSE;
      //      RUN_PROC(postEventProc);
      //      updateDisplays(ON_EVENT);

      //      if (Net->groupCritRequired && !critReached) break;

      if (++event < E->numEvents && ((V = E->event + event))) {
	if (E->set->loadEvent(V)) return TCL_ERROR;
	ticksOnEvent = 0;
	maxTime = chooseValue(V->maxTime, E->set->maxTime);
	minTime = chooseValue(V->minTime, E->set->minTime);
	graceTime = chooseValue(V->graceTime, E->set->graceTime);
	Net->inGracePeriod = (graceTime > 0.0);
	// RUN_PROC(preEventProc);
      }
    }
  }

  updateDisplays(ON_EXAMPLE);
  return TCL_OK;
}


/* sequence network type */
static flag initSequenceNet(Network N) {
  N->netTrain            = standardNetTrain;
  N->netTrainBatch       = standardNetTrainBatch;
  N->netTrainExample     = sequenceNetRunExample;
  N->netTrainExampleBack = NULL;
  N->netTrainTick        = sequenceNetForwardBackward;
  N->netTestBatch        = standardNetTestBatch;
  N->netTestExample      = sequenceNetRunExample;
  N->netTestTick         = sequenceNetForward;
  N->netRunExampleNum    = standardNetRunExampleNum;
  N->netRunExample       = sequenceNetRunExample;
  N->netRunTick          = sequenceNetForward;
  N->resetNet            = standardResetNet;
  N->saveWeights         = standardSaveWeights;
  N->loadWeights         = standardLoadWeights;
  return TCL_OK;
}


/******************************* User Initialization *************************/

/* This gets called while Lens is booting up */
flag userInit(void) {
  NetExtInfo = newObject("Network Extension", sizeof(struct netExt), 1,
			 extensionName, NULL, NULL);
  GroupExtInfo = newObject("Group Extension", sizeof(struct groupExt), 3,
			   extensionName, NULL, NULL);
  UnitExtInfo = newObject("Unit Extension", sizeof(struct unitExt), 5,
			  extensionName, NULL, NULL);
  BlockExtInfo = newObject("Block Extension", sizeof(struct blockExt), 7,
			   extensionName, NULL, NULL);
  ExSetExtInfo = newObject("Example Set Extension", sizeof(struct exSetExt), 3,
			   extensionName, NULL, NULL);
  ExampleExtInfo = newObject("Example Extension", sizeof(struct exampleExt), 5,
			     extensionName, NULL, NULL);
  EventExtInfo = newObject("Event Extension", sizeof(struct eventExt), 7,
			   extensionName, NULL, NULL);
  initNetExtInfo();
  initGroupExtInfo();
  initUnitExtInfo();
  initBlockExtInfo();
  initExSetExtInfo();
  initExampleExtInfo();
  initEventExtInfo();

  /* This creates a blank line in the list of commands produced by "help": */
  /* registerCommand(NULL, "", ""); */
  /* This creates the command "foo": */
  /* registerCommand(C_foo, "foo", "this command does some stuff"); */

  registerCommand(C_decodePMSP, "decodePMSP", "decode PMSP phonology");

  registerCommand(C_topoInitPositions, "topoInitPositions", "initialize topographic positions");
  registerCommand(C_topoSetPosition,   "topoSetPosition",    "set topographic positions");
  registerCommand(C_topoSetRow,        "topoSetRow",         "set topographic rows");
  registerCommand(C_topoSetCol,        "topoSetCol",         "set topographic cols");
  registerCommand(C_topoScaleDerivs,   "topoScaleDerivs",    "scale link derivatives by topographic distance");
  registerCommand(C_topoWeightDecay,   "topoWeightDecay",    "scale weight decay by topographic distance");
  registerCommand(C_limitOutWtMag,     "limitOutWtMag",      "limit total outgoing weight magnitudes");

  registerNetType("SEQUENCE", SEQUENCE, initSequenceNet);

  registerGroupType("DOT_PRODUCT_ONCE", DOT_PRODUCT_ONCE, GROUP_INPUT, 
		    dotProductOnceInputInit);
  registerGroupType("LOGISTIC_ONCE", LOGISTIC_ONCE, GROUP_OUTPUT, 
		    logisticOnceOutputInit);

  registerGroupType("GAP_CRIT",  GAP_CRIT,   GROUP,  gapGroupCriterionInit);
  registerGroupType("FIXED_CRIT", FIXED_CRIT, GROUP, fixedGroupCriterionInit);
  registerGroupType("MAXCHANGE_CRIT",  MAXCHANGE_CRIT,   GROUP,  maxChangeGroupCriterionInit);
  registerGroupType("AVECHANGE_CRIT",  AVECHANGE_CRIT,   GROUP,  aveChangeGroupCriterionInit);

  return TCL_OK;
}
