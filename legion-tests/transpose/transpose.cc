/* Copyright 2016 Stanford University
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <cstdio>
#include <cassert>
#include <cstdlib>
#include "legion.h"
using namespace LegionRuntime::HighLevel;
using namespace LegionRuntime::Accessor;

// Legion has a separate namespace which contains
// some useful abstractions for operations on arrays.
// Unsurprisingly it is called the Arrays namespace.
// We'll see an example of one of these operations
// in this example.
using namespace LegionRuntime::Arrays;

enum TaskIDs {
  TOP_LEVEL_TASK_ID,
  INIT_FIELD_TASK_ID,
  TRANSPOSE_TASK_ID,
  CHECK_TASK_ID,
};

enum FieldIDs {
  FID_VAL,
  FID_RES,
};

void top_level_task(const Task *task,
                    const std::vector<PhysicalRegion> &regions,
                    Context ctx, HighLevelRuntime *runtime)
{
  int num_elements = 4; 
  int num_subregions = num_elements;
  printf("Running vector transpose for %d elements...\n", num_elements);
  printf("Partitioning data into %d sub-regions...\n", num_subregions);

  // Create our logical regions using the same schemas as earlier examples
  Rect<1> elem_rect(Point<1>(0),Point<1>(num_elements-1));
  IndexSpace is_r = runtime->create_index_space(ctx, 
                          Domain::from_rect<1>(elem_rect));
  runtime->attach_name(is_r, "is_r");
  IndexSpace is_c = runtime->create_index_space(ctx, 
                          Domain::from_rect<1>(elem_rect));
  runtime->attach_name(is_c, "is_c");
  FieldSpace fs_r = runtime->create_field_space(ctx);
  runtime->attach_name(fs_r, "fs_r");

    FieldAllocator allocator_r = 
      runtime->create_field_allocator(ctx, fs_r);
    allocator_r.allocate_field(sizeof(int),FID_VAL);
    runtime->attach_name(fs_r, FID_VAL, "val");
  
  FieldSpace fs_c = runtime->create_field_space(ctx);
  runtime->attach_name(fs_c, "fs");

    FieldAllocator allocator_c = 
      runtime->create_field_allocator(ctx, fs_c);
    allocator_c.allocate_field(sizeof(int),FID_RES);
    runtime->attach_name(fs_c, FID_RES, "res");

  LogicalRegion input_lr = runtime->create_logical_region(ctx, is_r, fs_r);
  runtime->attach_name(input_lr, "input_lr");
  LogicalRegion output_lr = runtime->create_logical_region(ctx, is_c, fs_c);
  runtime->attach_name(output_lr, "output_lr");




  Rect<1> color_bounds(Point<1>(0),Point<1>(num_subregions-1));
  Domain color_domain = Domain::from_rect<1>(color_bounds);

  IndexPartition ip_c,ip_r;
  DomainColoring coloring, coloringcol;
  int index = 0;
  int num_elmts = num_elements/num_subregions;
  // We assume it divides 
  assert(num_elmts == 1);
  for (int color = 0; color < num_subregions; color++)
    {
      assert((index+num_elmts) <= num_elements);
      Rect<1> subrect(Point<1>(index),Point<1>(index+num_elmts-1));
      coloring[color] = Domain::from_rect<1>(subrect);
      assert((num_elements-index-1)>=0);
      Rect<1> subrectcol(Point<1>(num_elements-index-1),Point<1>(num_elements-index-1+num_elmts-1));
      coloringcol[color] =Domain::from_rect<1>(subrectcol);
      index += num_elmts;
    } 

  ip_r = runtime->create_index_partition(ctx, is_r, color_domain,
                                    coloring, true/*disjoint*/);
  ip_c = runtime->create_index_partition(ctx, is_c, color_domain,
                                    coloringcol, true/*disjoint*/);
  //ip_c = runtime->create_index_partition(ctx, is_c, color_domain,
  //                                  coloring, true/*disjoint*/);

  LogicalPartition input_lp = runtime->get_logical_partition(ctx, input_lr, ip_r);
  runtime->attach_name(input_lp, "input_lp");
  LogicalPartition output_lp = runtime->get_logical_partition(ctx, output_lr, ip_c);
  runtime->attach_name(output_lp, "output_lp");

  // Create our launch domain.  Note that is the same as color domain
  // as we are going to launch one task for each subregion we created.
  Domain launch_domain = color_domain; 
  ArgumentMap arg_map;

  for (int i = 0; i < num_elements; i++)
    {
     DomainPoint point = DomainPoint::from_point<1>(Point<1>(i));
     arg_map.set_point(point, TaskArgument(&i,sizeof(int)));
    }
  IndexLauncher init_launcher(INIT_FIELD_TASK_ID, launch_domain,
                              TaskArgument(NULL, 0), arg_map);
  // For index space task launches we don't want to have to explicitly
  // enumerate separate region requirements for all points in our launch
  // domain.  Instead Legion allows applications to place an upper bound
  // on privileges required by subtasks and then specify which privileges
  // each subtask receives using a projection function.  In the case of
  // the field initialization task, we say that all the subtasks will be
  // using some subregion of the LogicalPartition 'input_lp'.  Applications
  // may also specify upper bounds using logical regions and not partitions.
  //
  // The Legion implementation assumes that all all points in an index
  // space task launch request non-interfering privileges and for performance
  // reasons this is unchecked.  This means if two tasks in the same index
  // space are accessing aliased data, then they must either both be
  // with read-only or reduce privileges.
  //
  // When the runtime enumerates the launch_domain, it will invoke the
  // projection function for each point in the space and use the resulting
  // LogicalRegion computed for each point in the index space of tasks.
  // The projection ID '0' is reserved and corresponds to the identity 
  // function which simply zips the space of tasks with the space of
  // subregions in the partition.  Applications can register their own
  // projections functions via the 'register_region_projection' and
  // 'register_partition_projection' functions before starting 
  // the runtime similar to how tasks are registered.
  init_launcher.add_region_requirement(
      RegionRequirement(input_lp, 0/*projection ID*/, 
                        WRITE_DISCARD, EXCLUSIVE, input_lr));
  init_launcher.region_requirements[0].add_field(FID_VAL);
  runtime->execute_index_space(ctx, init_launcher);

// Init B
  ArgumentMap arg_mapb;
  for (int i = 0; i < num_elements; i++)
    {
     DomainPoint point = DomainPoint::from_point<1>(Point<1>(i));
     int val = i+num_elements;
     arg_mapb.set_point(point, TaskArgument(&val,sizeof(int)));
    }
  IndexLauncher init_launcherb(INIT_FIELD_TASK_ID, launch_domain,
                              TaskArgument(NULL, 0), arg_mapb);

  init_launcherb.add_region_requirement(
      RegionRequirement(output_lp, 0/*projection ID*/,
                        WRITE_DISCARD, EXCLUSIVE, output_lr));
  init_launcherb.region_requirements[0].add_field(FID_RES);
  runtime->execute_index_space(ctx, init_launcherb);


  ArgumentMap arg_mapc;
 
  // We launch the subtasks for performing the transpose computation
  // in a similar way to the initialize field tasks.  Note we
  // again make use of two RegionRequirements which use a
  // partition as the upper bound for the privileges for the task.
  IndexLauncher transpose_launcher(TRANSPOSE_TASK_ID, launch_domain,
                TaskArgument(NULL, 0), arg_mapc);
  transpose_launcher.add_region_requirement(
      RegionRequirement(input_lp, 0/*projection ID*/,
                        READ_ONLY, EXCLUSIVE, input_lr));
  transpose_launcher.region_requirements[0].add_field(FID_VAL);
  transpose_launcher.add_region_requirement(
      RegionRequirement(output_lp, 0/*projection ID*/,
                        READ_WRITE /*WRITE_DISCARD*/, EXCLUSIVE, output_lr));
  transpose_launcher.region_requirements[1].add_field(FID_RES);
  runtime->execute_index_space(ctx, transpose_launcher);
                    
  // While we could also issue parallel subtasks for the checking
  // task, we only issue a single task launch to illustrate an
  // important Legion concept.  Note the checking task operates
  // on the entire 'input_lr' and 'output_lr' regions and not
  // on the subregions.  Even though the previous tasks were
  // all operating on subregions, Legion will correctly compute
  // data dependences on all the subtasks that generated the
  // data in these two regions.  
  TaskLauncher check_launcher(CHECK_TASK_ID, TaskArgument(NULL, 0));
  check_launcher.add_region_requirement(
      RegionRequirement(input_lr, READ_ONLY, EXCLUSIVE, input_lr));
  check_launcher.region_requirements[0].add_field(FID_VAL);
  check_launcher.add_region_requirement(
      RegionRequirement(output_lr, READ_ONLY, EXCLUSIVE, output_lr));
  check_launcher.region_requirements[1].add_field(FID_RES);
  runtime->execute_task(ctx, check_launcher);

  runtime->destroy_logical_region(ctx, input_lr);
  runtime->destroy_logical_region(ctx, output_lr);
  runtime->destroy_field_space(ctx, fs_r);
  runtime->destroy_field_space(ctx, fs_c);
  runtime->destroy_index_space(ctx, is_r);
  runtime->destroy_index_space(ctx, is_c);
}

void init_field_task(const Task *task,
                     const std::vector<PhysicalRegion> &regions,
                     Context ctx, HighLevelRuntime *runtime)
{
  assert(regions.size() == 1); 
  assert(task->regions.size() == 1);
  assert(task->regions[0].privilege_fields.size() == 1);

  FieldID fid = *(task->regions[0].privilege_fields.begin());
  const int point = task->index_point.point_data[0];
  assert(task->local_arglen ==sizeof(int));
  int input = *((const int*) task->local_args);
  printf("Initializing field %d for block %d with local arg %d...\n", fid, point,input);

  RegionAccessor<AccessorType::Generic, int> acc =
    regions[0].get_field_accessor(fid).typeify<int>();

  // Note here that we get the domain for the subregion for
  // this task from the runtime which makes it safe for running
  // both as a single task and as part of an index space of tasks.
  Domain dom = runtime->get_index_space_domain(ctx, 
      task->regions[0].region.get_index_space());
  Rect<1> rect = dom.get_rect<1>();
  for (GenericPointInRectIterator<1> pir(rect); pir; pir++)
  {
    acc.write(DomainPoint::from_point<1>(pir.p), input);
    printf("Init:writting vaulue %d\n",input);
  }
}

void transpose_task(const Task *task,
                const std::vector<PhysicalRegion> &regions,
                Context ctx, HighLevelRuntime *runtime)
{
  assert(regions.size() == 2);
  assert(task->regions.size() == 2);
  //assert(task->arglen == sizeof(double));
  //const double alpha = *((const double*)task->args);
  const int point = task->index_point.point_data[0];

  //assert(task->local_arglen ==sizeof(int));
  //const int input = *((const int*)task->local_args);

  RegionAccessor<AccessorType::Generic, int> acc_a =
    regions[0].get_field_accessor(FID_VAL).typeify<int>();
  RegionAccessor<AccessorType::Generic, int> acc_b =
    regions[1].get_field_accessor(FID_RES).typeify<int>();
  printf("Running transpose computation for point %d...\n", 
          point);

  Domain dom = runtime->get_index_space_domain(ctx, 
      task->regions[0].region.get_index_space());
  Rect<1> rect = dom.get_rect<1>();
  for (GenericPointInRectIterator<1> pir(rect); pir; pir++)
  {
    int value = acc_a.read(DomainPoint::from_point<1>(pir.p));
    int value_old = acc_b.read(DomainPoint::from_point<1>(pir.p));
    acc_b.write(DomainPoint::from_point<1>(pir.p), value);
    printf("Swapping: Writing value %d on top of %d\n",value, value_old);
    int res = acc_b.read(DomainPoint::from_point<1>(pir.p));
    printf("Reading %d in B\n",res);
  }
}

void check_task(const Task *task,
                const std::vector<PhysicalRegion> &regions,
                Context ctx, HighLevelRuntime *runtime)
{
  assert(regions.size() == 2);
  assert(task->regions.size() == 2);
  RegionAccessor<AccessorType::Generic, int> acc_a =
    regions[0].get_field_accessor(FID_VAL).typeify<int>();
  RegionAccessor<AccessorType::Generic, int> acc_b =
    regions[1].get_field_accessor(FID_RES).typeify<int>();
  printf("Checking results...");
  Domain dom = runtime->get_index_space_domain(ctx, 
      task->regions[0].region.get_index_space());
  Rect<1> rect = dom.get_rect<1>();
  bool all_passed = true;
  int num_elems = rect.dim_size(0);
  for (GenericPointInRectIterator<1> pir(rect); pir; pir++)
  {
    int expected =  acc_a.read(DomainPoint::from_point<1>(pir.p));
    int indx = pir.p[0];
    int received = acc_b.read(DomainPoint::from_point<1>(num_elems - indx -1 ));
    printf("A[%d] is %d and B[%d] is %d\n",indx,expected,num_elems - indx -1,received);
    if (expected != received)
      all_passed = false;
  }
  if (all_passed)
    printf("SUCCESS!\n");
  else
    printf("FAILURE!\n");
}

int main(int argc, char **argv)
{
  HighLevelRuntime::set_top_level_task_id(TOP_LEVEL_TASK_ID);
  HighLevelRuntime::register_legion_task<top_level_task>(TOP_LEVEL_TASK_ID,
      Processor::LOC_PROC, true/*single*/, false/*index*/,
      AUTO_GENERATE_ID, TaskConfigOptions(), "top_level");
  // Note we mark that all of these tasks are capable of being
  // run both as single tasks and as index space tasks
  HighLevelRuntime::register_legion_task<init_field_task>(INIT_FIELD_TASK_ID,
      Processor::LOC_PROC, true/*single*/, true/*index*/,
      AUTO_GENERATE_ID, TaskConfigOptions(true), "init_field");
  HighLevelRuntime::register_legion_task<transpose_task>(TRANSPOSE_TASK_ID,
      Processor::LOC_PROC, true/*single*/, true/*index*/,
      AUTO_GENERATE_ID, TaskConfigOptions(true), "transpose");
  HighLevelRuntime::register_legion_task<check_task>(CHECK_TASK_ID,
      Processor::LOC_PROC, true/*single*/, true/*index*/,
      AUTO_GENERATE_ID, TaskConfigOptions(true), "check");

  return HighLevelRuntime::start(argc, argv);
}
