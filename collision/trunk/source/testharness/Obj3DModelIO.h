#ifndef __Obj3DModelIO_H__
#define __Obj3DModelIO_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "collision/CollisionStaticTrimesh.h"

CollisionStaticTrimesh * loadObjStaticModelFile(const char * filePath);

#ifdef __cplusplus
}
#endif
#endif
