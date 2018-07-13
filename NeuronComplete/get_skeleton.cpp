#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include "h5read.h"
#include "myskeletonwrapper.h"



bool skelexists(const char *filename)
{
  std::ifstream ifile(filename);
  return ifile;
}



int main(int argc, char** argv){
  std::time_t start_time = std::time(NULL);
  
  std::string segmentation_filename = argv[1];
  std::string segmentation_dataset_name = argv[2];
  unsigned long starting_id = atoi(argv[3]);

  // get prefix
  char *filename;
  filename = strrchr((char *) segmentation_filename.c_str(), '/');
  filename++;
  
  char root_filename[4096];
  strncpy(root_filename, filename, 4096);
  char *extp = strrchr(root_filename, '.');
  *extp = '\0';
  char *prefix = root_filename;
  
  // read the segmentation data
  H5Read segmentation(segmentation_filename.c_str(),segmentation_dataset_name.c_str());	
  uint* segmentation_data=NULL;	
  segmentation.readData(&segmentation_data);	
  
  int depth = segmentation.dim()[0];
  int height = segmentation.dim()[1];
  int width = segmentation.dim()[2];
  int nentries = depth * height * width;
  
  // get the maximum label
  unsigned long maximum_label = 0;
  for (int iv = 0; iv < nentries; ++iv)
    if (segmentation_data[iv] > maximum_label) maximum_label = segmentation_data[iv];
  
  // create the wrapper
  MySkeletonWrapper myskeleton(depth,height,width);
  for (unsigned long region_id = starting_id; region_id <= maximum_label; region_id++) {        
    // get the final output filename
    char output_filename[4096];
    sprintf(output_filename, "skeletons/%s/tree_%lu.swc", prefix, region_id);

    // see if this tree already exists
    if (skelexists(output_filename)) continue;

    // create a binary volume and compute the skeleton
    myskeleton.createBinaryVolume(segmentation_data, region_id);
    myskeleton.computeSkeleton();
    
    // save the tree
    myskeleton.saveTree(output_filename);

    // output stats
    printf("Computing skeleton for region %d in %lu seconds\n", region_id, std::time(NULL) - start_time);
    start_time = std::time(NULL);
  }
}
