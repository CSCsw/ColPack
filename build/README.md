#How to use

    ./$
    |-Example_ColPackAll              # all colpack function tested
    |-Example_General                 # general graph coloring 
    |-Example_Bicoloroing             # bipartite graph bi-coloring
    |-Example_PD2                     # bipartite grpah partial coloring
    |-Example_SMPGC                   # shared memory parallel graph coloring
    |-Example_PD2SMPGC                # shared memory parallel partial distance two coloring on bipartite graph
    |-Main                            # template for using


go into each directory and make will compile the code. The executable file is `ColPack`
    ColPack [-cmd <lists of arguments>] 
commands list
| |GeneralColoring|PartialColoring|BiColoring|ParallelGeneralColoring|ParallePartialColoring|
| ---- | ----------------- | -------------------|  
| Distance 1 coloring  | Partial distance-2 coloring  | Star bicoloring |  
| Distance 2 coloring | Partial distance-2 coloring  |   |  
| Star coloring |      |   |  
| Acyclic coloring   | |
|  Restricted star coloring| |
|  Triangular coloring| |

|-f| <list of graphnames>|
|-m| DISTANCE_ONE |
|  |


## usage

    cd Example_ColPackAll
    make
    $./ColPack -f <list of graphs> -m <list of methods> -o <list of orders>  [-v]

## general graph coloring

    cd Co
    make
    $./ColPack -f <graph_file_name> -o <ordering> -m <method> [-v]


## Bicoloring 
    
    make
    $./ColPack -f <adf>


## PartialDistanceTwo
    

## Parallel graph coloring for Distance One Coloring
adsfadf
contains
Ex

## Parallel graph coloring for partial distance two coloring
adf
adf
adf



