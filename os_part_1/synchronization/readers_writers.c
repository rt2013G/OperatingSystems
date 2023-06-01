// Psudocode for the readers-writers problem
// 1. Multiple readers can read from the buffer at the same time
// 2. Readers cannot read from the buffer if a writer is writing to it
// 3. Only one writer can write to the buffer at a time

begin
    reader()
    {
        Parbegin
            repeat
                if a writer is writing
                then 
                    { wait };
                if no other reader is reading
                then
                    if a writer is waiting
                    then
                        activate one waiting writer;
            forever
        Parend;
    }

    writer()
    {
        Parbegin
            repeat
            if a reader is reading or a writer is writing
            then
                { wait };
            { write }
            if a reader is waiting
            then
                activate all waiting readers;
            else if a writer is waiting
            then
                activate one waiting writer;
            forever
        Parend;
    }
end


// Solution 2 with semaphores
var
    totread : integer := 0;
    runread : integer := 0;
    totwrite : integer := 0;
    runwrite : integer := 0;
    reading : Semaphore := 0; // for signaling
    writing : Semaphore := 0; // for signaling
    sem_CS : Semaphore := 1; // for mutual exclusion
begin
    reader()
    {
        Parbegin
            repeat
                wait(sem_CS);
                totread := totread + 1;
                if runwrite = 0
                then
                    runread := runread + 1;
                    signal(reading);
                signal(sem_CS);
                wait(reading);
                { read }
                wait(sem_CS);
                runread := runread - 1;
                if runread = 0 and totwrite > 0
                then
                    runwrite := 1;
                    signal(writing);
                signal(sem_CS);
            forever
        Parend;
    }
    writer()
    {
        Parbegin
            repeat
                wait(sem_CS);
                totwrite := totwrite + 1;
                if runread = 0 and runwrite = 0
                then
                    runwrite := 1;
                    signal(writing);
                signal(sem_CS);
                wait(writing);
                { write }
                wait(sem_CS);
                runwrite := runwrite - 1;
                totwrite := totwrite - 1;
                while (runread < totread) do
                begin
                    runread := runread + 1;
                    signal(reading);
                end;
                if runread = 0 and totwrite > runwrite
                then
                    runwrite := 1;
                    signal(writing);
                signal(sem_CS);
            forever
        Parend;
    }
end