// Pseudocode for the dining philosophers problem
// 1. A philosopher can only eat if he has both forks
// 2. A solution must avoid deadlocks and livelocks
// This solution avoids deadlock and livelock but uses busy waiting

var successful : boolean;
repeat
    sucessful := false;
    while( not successful )
    {
        // CS begin
        if both forks are available then
            lift the forks on at a time
            successful := true;
        // CS end
        if successful = false
        then
            block (Pi);
        { eat }
        // CS begin
        put down both forks;
        // CS end
        if left philosopher is blocked
        then
            activate left philosopher;
        if right philosopher is blocked
        then
            activate right philosopher;
        { think }
    }
forever