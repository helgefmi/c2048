c2048
=====

c2048 is a [2048](http://saming.fr/p/2048/) game engine with AI, made in C.

c2048 uses bitmaps extensively, and should have a very fast board implementation. The AI is a simple depth-first search using `score + number_or_free_cells` as a heuristic.

If you want to change the parameters for the search algorithm, change `AI_DEPTH` and `AI_NUM_TRIES` in `ai.h`.

Shell arguments:
----------------

    --human  Play on the console, using hjkl to move around
    --test   Runs the testcases and does a performance test (nodes/sec)
    --ai     Runs infinitly many games in a row, while tracking stats (avg, max, nps, etc.)

If you run it with no arguments, it will be able to talk to [4096](https://github.com/matslindh/4096), a 2048 bot interface made by Mats Lindh:

    ../4096/4096/interface.py 0 ./2048
