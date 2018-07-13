#ifndef MYSKELETONWRAPPER_H
#define MYSKELETONWRAPPER_H

#include <string>
#include <string.h>
#include <stdlib.h>

#include "tz_utilities.h"
#include "zsegmentmaparray.h"
#include "zsuperpixelmaparray.h"
#include "tz_stack_lib.h"
#include "tz_image_io.h"
#include "tz_stack_attribute.h"
#include "tz_stack_document.h"
#include "tz_xml_utils.h"
#include "tz_stack_bwmorph.h"
#include "tz_stack_objlabel.h"
#include "tz_swc_tree.h"
#include "zswctree.h"
#include "zswcforest.h"
#include "tz_sp_grow.h"
#include "zspgrowparser.h"
#include "tz_stack_stat.h"
#include "tz_stack_math.h"
#include "tz_stack_lib.h"
#include "tz_stack_stat.h"
#include "zstackskeletonizer.h"
#include "c_stack.h"
#include "zobject3d.h"
#include "zobject3dscan.h"
#include "zfiletype.h"
#include "zstring.h"
#include "tz_constant.h"
#include "zargumentprocessor.h"
#include "tz_math.h"
#include "swc/zswcresampler.h"
#include "zjsonparser.h"

#include "skeleton_utility.h"




class MySkeletonWrapper{

    Stack *stack;
    ZStackSkeletonizer skeletonizer;
    ZSwcTree *wholeTree;

    int minObjSize;
    int minLen;

    std::vector<LeafStruct> endPts;
public:
    MySkeletonWrapper();
    MySkeletonWrapper(int pdepth, int pheight, int pwidth);
    ~MySkeletonWrapper(){
        if (stack->array != NULL)
            delete stack->array;
    }

    void resize(int pdepth, int pheight, int pwidth);
    void createBinaryVolume(uint* psegmentation_data, uint region_id);
    void computeSkeleton(void);

    void setMinObjSize(int pnewsz){ minObjSize = pnewsz;}
    void setMinLen(int pnewlen){ minLen = pnewlen;}
    void saveTree(std::string output_path);

    std::vector<LeafStruct>& getEndPoints(){return endPts;}

};

#endif // MYSKELETONWRAPPER_H

