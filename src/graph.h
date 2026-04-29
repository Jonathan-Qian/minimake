#ifndef GRAPH_H
#define GRAPH_H

#include "target.h"
#include "threadpool.h"

void build_graph(TargetList*);
int traverse(TaskQueue*, Target*);

#endif