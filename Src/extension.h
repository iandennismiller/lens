#ifndef EXTENSION_H
#define EXTENSION_H
#include "object.h"

extern char *VersionExt;

typedef struct netExt *NetExt;
typedef struct groupExt *GroupExt;
typedef struct unitExt *UnitExt;
typedef struct blockExt *BlockExt;
typedef struct exSetExt *ExSetExt;
typedef struct exampleExt *ExampleExt;
typedef struct eventExt *EventExt;

extern flag userInit(void);

extern flag initNetworkExtension(Network N);
extern flag resetNetworkExtension(Network N);
extern flag freeNetworkExtension(Network N);
extern flag copyNetworkExtension(Network from, Network to);

extern flag initGroupExtension(Group G);
extern flag freeGroupExtension(Group G);
extern flag copyGroupExtension(Group from, Group to);

extern flag initUnitExtension(Unit U);
extern flag freeUnitExtension(Unit U);
extern flag copyUnitExtension(Unit from, Unit to);

extern flag initBlockExtension(Block B);
extern flag freeBlockExtension(Block B);
extern flag copyBlockExtension(Block from, Block to);

extern flag initExSetExtension(ExampleSet S);
extern flag freeExSetExtension(ExampleSet S);
extern flag copyExSetExtension(ExampleSet from, ExampleSet to);

extern flag initExampleExtension(Example E);
extern flag freeExampleExtension(Example E);
extern flag copyExampleExtension(Example from, Example to);

extern flag initEventExtension(Event V);
extern flag freeEventExtension(Event V);
extern flag copyEventExtension(Event from, Event to);

extern ObjInfo NetExtInfo;
extern ObjInfo GroupExtInfo;
extern ObjInfo UnitExtInfo;
extern ObjInfo BlockExtInfo;
extern ObjInfo ExSetExtInfo;
extern ObjInfo ExampleExtInfo;
extern ObjInfo EventExtInfo;


// dcp

#define FOR_EACH_LINK_AND_SENDING_UNIT(U, proc) {\
  Link L, sL; Unit V; Block B, sB;\
  for (B = U->block, sB = B + U->numBlocks, L = U->incoming; B < sB; B++)\
    for (V = B->unit, sL = L + B->numUnits; L < sL; V++, L++)\
      {proc;}}

#define DIST(r1,c1,r2,c2) SQRT(((r1)-(r2))*((r1)-(r2))+((c1)-(c2))*((c1)-(c2)))
#define GAUSS(x,d) EXP(-((x)*(x))/((d)*(d)))


// autosequencer


extern flag makeDerivsSymmetric( void );
extern flag symmNetTrain( void );

extern void tanhEntropyError(Group G, GroupProc P);
extern void tanhEntropyErrorBack(Group G, GroupProc P);
extern void tanhEntropyErrorInit(Group G, GroupProc P);

extern void tanhDivergenceError(Group G, GroupProc P);
extern void tanhDivergenceErrorBack(Group G, GroupProc P);
extern void tanhDivergenceErrorInit(Group G, GroupProc P);

extern void asymmSquaredError(Group G, GroupProc P);
extern void asymmSquaredErrorBack(Group G, GroupProc P);
extern void asymmSquaredErrorInit(Group G, GroupProc P);

extern void cubedError(Group G, GroupProc P);
extern void cubedErrorBack(Group G, GroupProc P);
extern void cubedErrorInit(Group G, GroupProc P);

extern void ctkHardClampOutput(Group G, GroupProc P);
extern void ctkResetOutputs(Group G);
extern void ctkSoftMaxOutput(Group G, GroupProc P);
extern void ctkSoftMaxOutputBack(Group G, GroupProc P);
extern void ctkResetForwardIntegrators(Group G);
extern void ctkNoisyOutput(Group G, GroupProc P);

extern flag ctkForwardBackward(Event V);
extern flag ctkForward(Event V);
extern flag ctkStandardNetRunExample(Example E, flag (*tickProc)(Event), flag *correct);



#endif /* EXTENSION_H */
