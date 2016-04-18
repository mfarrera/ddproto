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

#include "legion.h"
#include "default_mapper.h"

// All of the important user-level objects live 
// in the high-level runtime namespace.
using namespace LegionRuntime::HighLevel;
using namespace LegionRuntime::Accessor;
using namespace LegionRuntime::Arrays;

// We use an enum to declare the IDs for user-level tasks
enum TaskID {
  MACHINE_INFO_ID,
};

// All single-launch tasks in Legion must have this signature with
// the extension that they can have different return values.
void machine_info_task(const Task *task, 
                      const std::vector<PhysicalRegion> &regions,
                      Context ctx, HighLevelRuntime *runtime)
{
  // A task runs just like a normal C++ function.
  Machine myMachine = Machine::get_machine();
  std::set<Processor> all_procs;
  myMachine.get_all_processors(all_procs);
  printf("Hello World! We are %lu procs\n",all_procs.size());
  for (std::set<Processor>::const_iterator it = all_procs.begin();
          it != all_procs.end(); it++)
    {
     Processor::Kind kind = it->kind();
      switch (kind)
      {
          case Processor::LOC_PROC:
          {
            printf("  Processor ID %x is CPU\n", it->id); 
            break;
          }
          case Processor::TOC_PROC:
          {
            printf("  Processor ID %x is GPU\n", it->id);
            break;
          }
          case Processor::UTIL_PROC:
          {
            printf("  Processor ID %x is utility\n", it->id);
            break;
          }
        default:
          assert(false);
      }
   }
   std::set<Memory> all_mems;
    myMachine.get_all_memories(all_mems);
    printf("There are %ld memories:\n", all_mems.size());
    for (std::set<Memory>::const_iterator it = all_mems.begin();
          it != all_mems.end(); it++)
    {
      Memory::Kind kind = it->kind();
      size_t memory_size_in_kb = it->capacity() >> 10;
      switch (kind)
      {
        case Memory::GLOBAL_MEM:
          {
            printf("  GASNet Global Memory ID %x has %ld KB\n", 
                    it->id, memory_size_in_kb);
            break;
          }
        case Memory::SYSTEM_MEM:
          {
            printf("  System Memory ID %x has %ld KB\n",
                    it->id, memory_size_in_kb);
            break;
          }
        case Memory::REGDMA_MEM:
          {
            printf("  Pinned Memory ID %x has %ld KB\n",
                    it->id, memory_size_in_kb);
            break;
          }
        case Memory::SOCKET_MEM:
          {
            printf("  Socket Memory ID %x has %ld KB\n",
                    it->id, memory_size_in_kb);
            break;
          }

         case Memory::Z_COPY_MEM:
          {
            printf("  Zero-Copy Memory ID %x has %ld KB\n",
                    it->id, memory_size_in_kb);
            break;
          }
        case Memory::GPU_FB_MEM:
          {
            printf("  GPU Frame Buffer Memory ID %x has %ld KB\n",
                    it->id, memory_size_in_kb);
            break;
          }
        case Memory::DISK_MEM:
          {
            printf("  Disk Memory ID %x has %ld KB\n",
                    it->id, memory_size_in_kb);
            break;
          }
          case Memory::HDF_MEM:
          {
            printf("  HDF Memory ID %x has %ld KB\n",
                    it->id, memory_size_in_kb);
            break;
          }
        case Memory::FILE_MEM:
          {
            printf("  File Memory ID %x has %ld KB\n",
                    it->id, memory_size_in_kb);
            break;
          }
        case Memory::LEVEL3_CACHE:
          {
            printf("  Level 3 Cache ID %x has %ld KB\n",
                    it->id, memory_size_in_kb);
            break;
          }
       case Memory::LEVEL2_CACHE:
          {
            printf("  Level 2 Cache ID %x has %ld KB\n",
                    it->id, memory_size_in_kb);
            break;
          }
        case Memory::LEVEL1_CACHE:
          {
            printf("  Level 1 Cache ID %x has %ld KB\n",
                    it->id, memory_size_in_kb);
            break;
          }
        default:
          printf(" unknown memory type %d", kind);
          assert(false);
      }
    }
    /*std::set<Memory> vis_mems;
    myMachine.get_visible_memories(local_proc, vis_mems);
    printf("There are %ld memories visible from processor %x\n",
            vis_mems.size(), local_proc.id);
    for (std::set<Memory>::const_iterator it = vis_mems.begin();
          it != vis_mems.end(); it++)
    {
        std::vector<ProcessorMemoryAffinity> affinities;
      int results = 
        myMachine.get_proc_mem_affinity(affinities, local_proc, *it);
        assert(results == 1);
      printf("  Memory %x has bandwidth %d and latency %d\n",
              it->id, affinities[0].bandwidth, affinities[0].latency);
    }
    */
  
}

// We have a main function just like a standard C++ program.
// Once we start the runtime, it will begin running the top-level task.
int main(int argc, char **argv)
{
  // Before starting the Legion runtime, you first have to tell it
  // what the ID is for the top-level task.
  HighLevelRuntime::set_top_level_task_id(MACHINE_INFO_ID);
  // Before starting the Legion runtime, all possible tasks that the
  // runtime can potentially run must be registered with the runtime.
  // The function pointer is passed as a template argument.  The second
  // argument specifies the kind of processor on which the task can be
  // run: latency optimized cores (LOC) aka CPUs or throughput optimized
  // cores (TOC) aka GPUs.  The last two arguments specify whether the
  // task can be run as a single task or an index space task (covered
  // in more detail in later examples).  The top-level task must always
  // be able to be run as a single task.
  HighLevelRuntime::register_legion_task<machine_info_task>(MACHINE_INFO_ID,
      Processor::LOC_PROC, true/*single*/, false/*index*/);

  // Now we're ready to start the runtime, so tell it to begin the
  // execution.  We'll never return from this call, but its return 
  // signature will return an int to satisfy the type checker.
  return HighLevelRuntime::start(argc, argv);
}
